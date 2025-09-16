<script lang="ts">
	import type { Wallet, Transaction } from "$lib/index";
	import {
		record_transaction,
		record_transfer,
		update_record,
		delete_record,
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
	let selectedWalletName: string = $state("Select a wallet");
	let selectedType: string = $state("Select a type");
	let selectedCategory: string = $state("Select a category");
	let selected_RwalletName: string = $state("Select a wallet");
	let sourceCurrency: string = $state("");
	let destinationCurrency: string = $state("");
	let initialWallet: string = $state("");
	let initialRWallet: string = $state("");
	let initialType: string = $state("");
	let initialAmount: number = $state(0);
	const types: string[] = ["INCOME", "EXPENSE", "TRANSFER"];
	const categories: string[] = [
		"Salary",
		"Refund",
		"Gift",
		"Food",
		"Groceries",
		"Transportation",
		"Fuel",
		"Utilities",
		"Rent",
		"Health",
		"Education",
		"Shopping",
		"Entertainment",
		"Donation",
		"Others",
	];

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
		to_update.is_archived = initialAmount;
		to_update.recorded_at = initialWallet + "T" + initialType;
		to_update.updated_at =
			initialRWallet + "T" + to_update.related_wallet_name;
		({ success_popup, error_popup } = await update_record(to_update));
		editing = false;
		initialAmount = 0;
		initialRWallet = "";
		initialType = "";
		initialWallet = "";
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
		selectedType = "Select a type";
		selectedCategory = "Select a category";
		selectedWalletName = "Select a wallet";
		selected_RwalletName = "Select a wallet";
		sourceCurrency = "";
		destinationCurrency = "";
		await refresh_wrap();
		fade_alert();
	}
	async function delete_record_wrap(to_delete: Transaction) {
		({ success_popup, error_popup } = await delete_record(to_delete));
		await refresh_wrap();
		fade_alert();
	}
	function start_edit_transaction(transaction: Transaction) {
		editing = true;
		a_record.id = transaction.id;
		a_record = { ...transaction };
		selectedCategory = transaction.category;
		selectedType = transaction.type;
		selectedWalletName = transaction.wallet_name;
		selected_RwalletName = transaction.related_wallet_name;
		sourceCurrency = fetched_wallets.find(
			(wallet) => wallet.name === transaction.wallet_name
		)?.currency as string;
		initialAmount = transaction.amount;
		initialRWallet = transaction.related_wallet_name;
		initialWallet = transaction.wallet_name;
		initialType = transaction.type;
	}
	async function create_new_transaction() {
		if (a_record.wallet_name && a_record.category) {
			if (a_record.type.toLocaleLowerCase() === "transfer") {
				if (
					sourceCurrency.toLocaleLowerCase() ===
					destinationCurrency.toLocaleLowerCase()
				) {
					await record_transfer_wrap(a_record);
				} else {
					error_popup =
						"Cannot transfer between different currencies!!";
					fade_alert();
				}
			} else {
				a_record.related_wallet_name = a_record.wallet_name;
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
			selectedType = "Select a type";
			selectedCategory = "Select a category";
			selectedWalletName = "Select a wallet";
			selected_RwalletName = "Select a wallet";
			sourceCurrency = "";
			destinationCurrency = "";
		} else {
			error_popup = "Please fill in all required fields";
			fade_alert();
		}
	}
	function format_date(date_string: string): string {
		return new Date(date_string).toLocaleDateString();
	}
	function format_amount(amount: number, type: string): string {
		const formatted = amount.toFixed(2);
		return `${type.toLocaleLowerCase() === "income" ? "+" : "-"}${formatted}`;
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
	{#if is_creating || editing}
		<div class="mb-6 bg-base-100 p-6 rounded-lg border shadow-sm">
			<h3 class="text-lg font-semibold mb-4">
				{is_creating ? "Create New" : "Update"} Transaction
			</h3>
			<div
				class="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-4 mb-4"
			>
				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Wallet Name
					</p>
					<div class="dropdown w-full">
						<div tabindex="0" role="button" class="btn w-full">
							<span class="mr-auto">{selectedWalletName}</span>
						</div>
						<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
						<ul
							tabindex="0"
							class="dropdown-content menu bg-base-100 rounded-box z-1 w-full p-2 shadow-sm"
						>
							{#each fetched_wallets as wallet}
								<!-- svelte-ignore a11y_missing_attribute -->
								<!-- svelte-ignore a11y_click_events_have_key_events -->
								<!-- svelte-ignore a11y_no_static_element_interactions -->
								<li>
									<a
										onclick={() => (
											(a_record.wallet_name =
												wallet.name),
											(selectedWalletName = wallet.name),
											(sourceCurrency = wallet.currency),
											(
												document.activeElement as HTMLElement
											)?.blur()
										)}
									>
										{wallet.name}</a
									>
								</li>
							{/each}
						</ul>
					</div>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Type
					</p>
					<div class="dropdown w-full">
						<div tabindex="0" role="button" class="btn w-full">
							<span class="mr-auto">{selectedType}</span>
						</div>
						<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
						<ul
							tabindex="0"
							class="dropdown-content menu bg-base-100 rounded-box z-1 w-full p-2 shadow-sm"
						>
							{#each types as type}
								<!-- svelte-ignore a11y_missing_attribute -->
								<!-- svelte-ignore a11y_click_events_have_key_events -->
								<!-- svelte-ignore a11y_no_static_element_interactions -->
								<li>
									<a
										onclick={() => (
											(a_record.type = type),
											(selectedType = type),
											(
												document.activeElement as HTMLElement
											)?.blur()
										)}>{type}</a
									>
								</li>
							{/each}
						</ul>
					</div>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Category
					</p>
					<div class="dropdown w-full">
						<div tabindex="0" role="button" class="btn w-full">
							<span class="mr-auto">{selectedCategory}</span>
						</div>
						<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
						<ul
							tabindex="0"
							class="dropdown-content menu bg-base-100 rounded-box z-1 w-full p-2 shadow-sm"
						>
							{#each categories as category}
								<!-- svelte-ignore a11y_missing_attribute -->
								<!-- svelte-ignore a11y_click_events_have_key_events -->
								<!-- svelte-ignore a11y_no_static_element_interactions -->
								<li>
									<a
										onclick={() => (
											(a_record.category = category),
											(selectedCategory = category),
											(
												document.activeElement as HTMLElement
											)?.blur()
										)}>{category}</a
									>
								</li>
							{/each}
						</ul>
					</div>
				</div>

				<div>
					<p class="block text-sm bg-base-100 font-medium text-base-content mb-1">
						Description
					</p>
					<input
						type="text"
						bind:value={a_record.description}
						placeholder="Enter description"
						class="w-full px-3 py-2 border-2 border-neutral/20 rounded-md focus:outline-none focus:ring-1 focus:ring-warning"
					/>
				</div>

				<div>
					<p class="block text-sm font-medium text-base-content mb-1">
						Amount
					</p>
					<div class="flex-1 flex gap-x-1">
						<div
							class="badge badge-soft h-full badge-warning font-bold p-3"
						>
							{sourceCurrency}
						</div>
						<input
							type="number"
							bind:value={a_record.amount}
							placeholder={sourceCurrency}
							class="w-full px-3 py-2 border-2 border-neutral/20 rounded-md focus:outline-none focus:ring-1 focus:ring-warning"
						/>
					</div>
				</div>

				{#if a_record.type.toLocaleLowerCase() === "transfer"}
					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Related Wallet
						</p>
						<div class="dropdown w-full">
							<div tabindex="0" role="button" class="btn w-full">
								<span class="mr-auto"
									>{selected_RwalletName}</span
								>
							</div>
							<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
							<ul
								tabindex="0"
								class="dropdown-content menu bg-base-100 rounded-box z-1 w-full p-2 shadow-sm"
							>
								{#each fetched_wallets as wallet}
									<!-- svelte-ignore a11y_missing_attribute -->
									<!-- svelte-ignore a11y_click_events_have_key_events -->
									<!-- svelte-ignore a11y_no_static_element_interactions -->
									<li>
										<a
											onclick={() => (
												(a_record.related_wallet_name =
													wallet.name),
												(selected_RwalletName =
													wallet.name),
												(destinationCurrency =
													wallet.currency),
												(
													document.activeElement as HTMLElement
												)?.blur()
											)}
										>
											{wallet.name}</a
										>
									</li>
								{/each}
							</ul>
						</div>
					</div>
				{/if}
			</div>

			<div class="flex justify-end mr-5 gap-4">
				<button
					onclick={() =>
						editing
							? update_record_wrap(a_record)
							: create_new_transaction()}
					class="flex items-center cursor-pointer gap-2 bg-success text-base-100 px-4 py-2 rounded-xl hover:bg-success transition-colors"
				>
					<i class="fas fa-save"></i>
					{is_creating ? "Create" : "Update"}
				</button>
				<button
					onclick={() => ((is_creating = false), (editing = false))}
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
					{#each fetched_transactions.filter(transaction => !transaction.is_archived) as transaction (transaction.id)}
						<tr
							class="hover:bg-base-100 bg-base-100 border-b rounded-b-4xl border-b-base-content/20 transition-colors"
						>
							<td class="px-6 py-3 text-sm text-base-content">
								{format_date(transaction.recorded_at)}
							</td>
							<td
								class="px-4 py-3 text-sm font-medium text-base-content"
							>
								{#if transaction.type.toLowerCase() === "transfer"}
									<span class="text-error"
										>{transaction.wallet_name}</span
									>
									→
									<span class="text-success"
										>{transaction.related_wallet_name}</span
									>
								{:else}
									<span>{transaction.wallet_name}</span>
								{/if}
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
								<label
									for="description_modal"
									class="btn outline-none bg-transparent border-none max-w-46 font-normal justify-start truncate"
								>
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
												class="btn rounded-2xl hover:bg-error/70"
												>Close</label
											>
										</div>
									</div>
								</div>
							</td>
							<td class="px-4 py-3 text-sm font-medium">
								<span class={get_type_color(transaction.type)}>
									{format_amount(
										transaction.amount,
										transaction.type
									)}
									{fetched_wallets.find(
										(wallet) =>
											wallet.name ===
											transaction.wallet_name
									)?.currency}
								</span>
							</td>
							<td class="px-4 py-3">
								<div class="flex gap-2">
									<button
										onclick={() =>
											start_edit_transaction(transaction)}
										class="p-2 cursor-pointer text-primary hover:bg-primary-content rounded-4xl transition-colors"
										aria-label="Edit Transaction"
									>
										<i
											class="fas fa-pen-to-square rounded-4xl"
										></i>
									</button>
									<label
										for="my_modal_6"
										class="btn p-2.5 cursor-pointer text-error hover:bg-secondary-content rounded-4xl transition-colors border-none btn-ghost"
									>
										<i class="fas fa-trash rounded-4xl"></i>
									</label>
									<input
										type="checkbox"
										id="my_modal_6"
										class="modal-toggle"
									/>
									<div class="modal" role="dialog">
										<div class="modal-box">
											<h3
												class="text-lg text-error font-bold"
											>
												Warning
											</h3>
											<p class="mt-3">
												Are you sure you want to delete
												this transaction?
											</p>
											<div class="modal-action">
												<button
													class="hover:bg-error btn rounded-xl transition-colors transition-duration-200"
													onclick={() =>
														delete_record_wrap(
															transaction
														)}
												>
													Confirm
												</button>
												<label
													for="my_modal_6"
													class="btn rounded-xl"
													>Cancel</label
												>
											</div>
										</div>
									</div>
								</div>
							</td>
						</tr>
					{/each}
				</tbody>
			</table>
		</div>
	</div>
	<!-- Empty state -->
	{#if fetched_transactions.filter(t => !t.is_archived).length === 0}
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
