<script lang="ts">
	import type { Wallet, Transaction } from "$lib/index";
	import {
		create_wallet,
		record_transaction,
		record_transfer,
		update_record,
		update_wallet,
		update_transfer,
		get_wallets,
		get_records,
		delete_record,
		delete_wallet,
		trash_wallet,
		restore_wallet,
		refresh,
	} from "$lib/index";
	import { onMount } from "svelte";

	let editing: boolean = $state(false);
	let is_creating: boolean = $state(false);
	let a_record: Transaction = $state({
		id: 0,
		wallet_name: "",
		type: "",
		category: "",
		description: "",
		amount: 0,
		related_wallet_name: "",
		recorded_at: "",
		updated_at: "",
		is_archived: false,
	});
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);

	onMount(async () => refresh_wrap());
	function fade_alert() {
		setTimeout(() => {
			success_popup = "";
			error_popup = "";
		}, 3000);
	}
	async function refresh_wrap() {
		const { wallets, records } = await refresh();
		fetched_wallets = wallets;
		fetched_transactions = records;
	}
	async function record_transaction_wrap(to_record: Transaction) {
		({ success_popup, error_popup } = await record_transaction(to_record));
		await refresh_wrap();
		fade_alert();
	}
	async function record_transfer_wrap(to_record: Transaction) {
		({ success_popup, error_popup } = await record_transfer(to_record));
		await refresh_wrap();
		fade_alert();
	}
	async function update_record_wrap(to_update: Transaction) {
		if (
			to_update.type.toLocaleLowerCase() === "expense" ||
			to_update.type.toLocaleLowerCase() === "income"
		) {
			({ success_popup, error_popup } = await update_record(to_update));
		} else {
			({ success_popup, error_popup } = await update_transfer(to_update));
		}
		editing = false;
		await refresh_wrap();
		fade_alert();
	}
	async function get_wallets_wrap() {
		fetched_wallets = await get_wallets();
		if (fetched_wallets.length === 0) error_popup = "No wallets found";
	}
	async function get_records_wrap() {
		fetched_transactions = await get_records();
		if (fetched_transactions.length === 0)
			error_popup = "No transactions found";
	}
	async function delete_record_wrap(to_delete: Transaction) {
		({ success_popup, error_popup } = await delete_record(to_delete));
		await refresh_wrap();
		fade_alert();
	}
	// NEW CRUD FUNCTIONS FOR TRANSACTIONS

	// Start editing a transaction
	function start_edit_transaction(transaction: Transaction) {
		editing = true;
		a_record.id = transaction.id;
		a_record = { ...transaction };
	}

	// Cancel editing
	function cancel_edit_transaction() {
		a_record = {
			id: 0,
			wallet_name: "",
			type: "",
			category: "",
			description: "",
			amount: 0,
			related_wallet_name: "",
			recorded_at: "",
			updated_at: "",
			is_archived: false,
		};
		editing = false;
	}

	// Delete transaction with confirmation
	async function delete_transaction_confirm(transaction: Transaction) {
		if (confirm("Are you sure you want to delete this transaction?")) {
			await delete_record_wrap(transaction);
		}
	}

	// Create new transaction
	async function create_new_transaction() {
		if (a_record.wallet_name && a_record.category && a_record.description) {
			if (a_record.type.toLocaleLowerCase() === "transfer") {
				await record_transfer_wrap(a_record);
			} else {
				await record_transaction_wrap(a_record);
			}

			// Reset form
			a_record = {
				id: 0,
				wallet_name: "",
				type: "",
				category: "",
				description: "",
				amount: 0,
				related_wallet_name: "",
				recorded_at: "",
				updated_at: "",
				is_archived: false,
			};
			is_creating = false;
		} else {
			error_popup = "Please fill in all required fields";
			fade_alert();
		}
	}

	// Utility functions
	function format_date(date_string: string): string {
		return new Date(date_string).toLocaleDateString();
	}

	function format_amount(amount: number, type: string): string {
		const formatted = amount.toFixed(2);
		if (type === "EXPENSE") return `-$${formatted}`;
		if (type === "INCOME") return `+$${formatted}`;
		return `$${formatted}`;
	}

	function get_type_color(type: string): string {
		switch (type) {
			case "EXPENSE":
				return "text-error";
			case "INCOME":
				return "text-success";
			case "TRANSFER":
				return "text-primary";
			default:
				return "text-base-content";
		}
	}
</script>

<main class="min-h-screen p-6 mx-auto space-y-6 bg-base-100 text-base-content">
	<div>
		{#if success_popup}<div class="alert alert-success">
				{success_popup}
			</div>{/if}
		{#if error_popup}<div class="alert alert-error">{error_popup}</div>{/if}
	</div>

	<div class="flex justify-between items-center mb-10">
		<h1 class="text-4xl font-bold text-base-content">
			Transaction Management
		</h1>
		<button
			onclick={() => (is_creating = true)}
			class="flex items-center cursor-pointer gap-2 bg-warning text-base-100 px-4 py-2 rounded-lg hover:drop-shadow-xl transition-colors"
		>
			<i class="fas fa-plus"></i>
			Add Transaction
		</button>
	</div>

	<!-- Create new transaction form -->
	{#if is_creating}
		<div class="mb-6 bg-base-100 p-6 rounded-lg border shadow-sm">
			<h3 class="text-lg font-semibold mb-4">Create New Transaction</h3>
			<div
				class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 mb-4"
			>
				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Wallet Name
					</p>
					<select
						bind:value={a_record.wallet_name}
						class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
					>
						<option value="">Select Wallet</option>
						{#each fetched_wallets as wallet}
							<option value={wallet.name}>{wallet.name}</option>
						{/each}
					</select>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Type
					</p>
					<select
						bind:value={a_record.type}
						class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
					>
						<option value="EXPENSE">EXPENSE</option>
						<option value="INCOME">INCOME</option>
						<option value="TRANSFER">TRANSFER</option>
					</select>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Category
					</p>
					<input
						type="text"
						bind:value={a_record.category}
						placeholder="Enter category"
						class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
					/>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Description
					</p>
					<input
						type="text"
						bind:value={a_record.description}
						placeholder="Enter description"
						class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
					/>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Amount
					</p>
					<input
						type="number"
						step="0.01"
						bind:value={a_record.amount}
						placeholder="0.00"
						class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
					/>
				</div>

				{#if is_creating}
					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Related Wallet
						</p>
						<select
							bind:value={a_record.related_wallet_name}
							class="w-full px-3 py-2 border rounded-xl focus:outline-none focus:ring-1 focus:ring-warning"
						>
							<option value="">Select Related Wallet</option>
							{#each fetched_wallets as wallet}
								<option value={wallet.name}
									>{wallet.name}</option
								>
							{/each}
						</select>
					</div>
				{/if}
			</div>

			<div class="flex gap-2">
				<button
					onclick={create_new_transaction}
					class="flex items-center cursor-pointer gap-2 bg-success text-base-100 px-4 py-2 rounded-xl hover:bg-success transition-colors"
				>
					<i class="fas fa-save"></i>
					Create
				</button>
				<button
					onclick={() => (is_creating = false)}
					class="flex items-center cursor-pointer gap-2 bg-base-content/50 text-base-100 px-4 py-2 rounded-xl hover:bg-error/80 transition-colors"
				>
					<i class="fas fa-multiply"></i>
					Cancel
				</button>
			</div>
		</div>
	{/if}

	<!-- Transactions Table -->
	<div class="bg-base-100 rounded-lg shadow-md overflow-hidden">
		<div class="overflow-x-auto">
			<table class="w-full">
				<thead class="bg-warning/30 text-base-content/80">
					<tr>
						<th class="px-8 py-3 text-left text-sm font-bold"
							>Date</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Wallet</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Type</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Category</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Description</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Amount</th
						>
						<th class="px-4 py-3 text-left text-sm font-bold"
							>Actions</th
						>
					</tr>
				</thead>
				<tbody class="divide-y divide-base-300">
					{#each fetched_transactions as transaction (transaction.id)}
						<tr
							class="hover:bg-base-100 border-b border-b-base-content/20 transition-colors"
						>
							{#if editing && a_record.id === transaction.id && a_record}
								<!-- Edit Mode Row -->
								<td
									class="px-4 py-3 text-sm text-base-content/60"
								>
									{transaction.recorded_at}
								</td>
								<td class="px-4 py-3">
									<select
										bind:value={a_record.wallet_name}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-warning"
									>
										{#each fetched_wallets as wallet}
											<option value={wallet.name}
												>{wallet.name}</option
											>
										{/each}
									</select>
								</td>
								<td class="px-4 py-3">
									<select
										bind:value={a_record.type}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-warning"
									>
										<option value="EXPENSE">Expense</option>
										<option value="INCOME">Income</option>
										<option value="TRANSFER"
											>Transfer</option
										>
									</select>
								</td>
								<td class="px-4 py-3">
									<input
										type="text"
										bind:value={a_record.category}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-warning"
									/>
								</td>
								<td class="px-4 py-3">
									<input
										type="text"
										bind:value={a_record.description}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-warning"
									/>
								</td>
								<td class="px-4 py-3">
									<input
										type="number"
										step="0.01"
										bind:value={a_record.amount}
										class="w-full px-2 py-1 text-sm border rounded focus:outline-none focus:ring-2 focus:ring-warning"
									/>
								</td>
								<td class="px-4 py-3">
									<div class="flex gap-1">
										<button
											onclick={() =>
												update_record_wrap(a_record)}
											class="p-1 text-success hover:bg-success/40 rounded-4xl transition-colors"
											aria-label="Save"
										>
											<i
												class="fas fa-save p-2 rounded-4xl"
											></i>
										</button>
										<button
											onclick={cancel_edit_transaction}
											class="p-1 text-base-content hover:bg-base-300 rounded-4xl transition-colors"
											aria-label="Cancel"
										>
											<i
												class="fas fa-multiply p-2 rounded-4xl"
											></i>
										</button>
									</div>
								</td>
							{:else}
								<!-- View Mode Row -->
								<td class="px-6 py-3 text-sm text-base-content">
									{format_date(transaction.recorded_at)}
								</td>
								<td
									class="px-4 py-3 text-sm font-medium text-base-content"
								>
									{transaction.wallet_name}
								</td>
								<td class="px-4 py-3">
									<span
										class="text-sm font-medium capitalize {get_type_color(
											transaction.type
										)}"
									>
										{transaction.type}
									</span>
								</td>
								<td class="px-4 py-3 text-sm text-base-content">
									{transaction.category}
								</td>
								<td class="px-4 py-3 text-sm text-base-content">
									<label for="description_modal" class="btn outline-none bg-transparent border-none max-w-46 font-normal justify-start truncate">
										{transaction.description}
									</label>
									<input
										type="checkbox"
										id="description_modal"
										class="modal-toggle"
									/>
									<div class="modal" role="dialog">
										<div class="modal-box">
											<h3 class="text-2xl font-bold">
												Description
											</h3>
											<p class="py-4">
												{transaction.description}
											</p>
											<div class="modal-action mt-0">
												<label
													for="description_modal"
													class="btn">Close</label
												>
											</div>
										</div>
									</div>
								</td>
								<td class="px-4 py-3 text-sm font-medium">
									<span
										class={get_type_color(transaction.type)}
									>
										{format_amount(
											transaction.amount,
											transaction.type
										)}
									</span>
								</td>
								<td class="px-4 py-3">
									<div class="flex gap-2">
										<button
											onclick={() =>
												start_edit_transaction(
													transaction
												)}
											class="p-2 cursor-pointer text-primary hover:bg-primary-content rounded-4xl transition-colors"
											aria-label="Edit Transaction"
										>
											<i
												class="fas fa-pen-to-square rounded-4xl"
											></i>
										</button>
										<button
											onclick={() =>
												delete_transaction_confirm(
													transaction
												)}
											class="p-2 cursor-pointer text-error hover:bg-secondary-content rounded-4xl transition-colors"
											aria-label="Delete Transaction"
										>
											<i class="fas fa-trash rounded-4xl"
											></i>
										</button>
									</div>
								</td>
							{/if}
						</tr>
					{/each}
				</tbody>
			</table>
		</div>
	</div>
	<!-- Empty state -->
	{#if fetched_transactions.length === 0}
		<div class="text-center py-2">
			<i class="fas fa-file-alt text-error mb-2 text-5xl"></i>
			<p class="text-base-content text-lg">No transactions found</p>
			<p class="text-base-neutral text-sm">
				<!-- svelte-ignore a11y_missing_attribute -->
				<!-- svelte-ignore a11y_click_events_have_key_events -->
				<!-- svelte-ignore a11y_no_static_element_interactions -->
				Click
				<a
					class="hover:text-warning cursor-pointer transition-all duration-300"
					onclick={() => {
						is_creating = true;
					}}>Add Transaction</a
				> to create your first transaction.
			</p>
		</div>
	{/if}
</main>
