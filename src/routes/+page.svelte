<script lang="ts">
	import type { Wallet, Transaction } from "$lib/index";
	import { refresh } from "$lib/index";
	import { onMount } from "svelte";

	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);

	onMount(async () => refresh_wrap());
	async function refresh_wrap() {
		const { wallets, records } = await refresh();
		fetched_wallets = wallets;
		fetched_transactions = records;
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

<main class="mt-3">
	<h1 class="font-extrabold text-4xl p-6 text-base-content">Overview</h1>
	<div class="flex flex-col gap-y-3">
		<div
			class="flex flex-col gap-y-4 ml-6 mr-6 overflow-x-auto border border-base-300 shadow-sm rounded-xl p-5"
		>
			<h2 class="text-xl font-semibold">Wallets</h2>
			<div class="flex-row inline-flex gap-x-6 overflow-x">
				{#each fetched_wallets.filter((wallet) => wallet.is_active === true) as wallet (wallet.id)}
					<a href="#/allWallets">
						<div
							class="card !bg-primary/60 text-primary-content w-80 h-40"
						>
							<div class="card-body">
								<div class="flex">
									<i class="fa-solid fa-magnet text-xl"></i>
									<h2
										class="card-title ml-auto mr-6 font-extrabold text-2xl"
									>
										{wallet.name}
									</h2>
								</div>
								<div class="mt-6 flex font-semibold">
									<p class="text-3xl">
										{wallet.currency}
										{wallet.balance}
									</p>
								</div>
							</div>
						</div>
					</a>
				{/each}
				{#if fetched_wallets.filter((w) => w.is_active === true).length === 0}
					<div class="text-center py-2 w-full">
						<i class="fas fa-file-alt text-error mb-2 text-5xl"></i>
						<p class="text-base-content text-lg">
							No wallets found
						</p>
						<p class="text-base-neutral text-sm">
							<!-- svelte-ignore a11y_missing_attribute -->
							<!-- svelte-ignore a11y_click_events_have_key_events -->
							<!-- svelte-ignore a11y_no_static_element_interactions -->
							Click
							<a
								href="#/allWallets"
								class="hover:text-warning cursor-pointer transition-all duration-300"
							>
								Add Wallet</a
							> to create your first wallet.
						</p>
					</div>
				{/if}
			</div>
		</div>
		<div
			class="flex flex-col gap-y-4 ml-6 mr-6 border border-base-300 max-h-145 overflow-y-auto shadow-sm rounded-xl p-5"
		>
			<h2 class="text-xl font-semibold">Recent Transactions</h2>
			<div class="bg-base-100 rounded-lg shadow-md overflow-y-auto overflow-hidden">
				<div class=" overflow-y-auto">
					<table class="w-full">
						<thead class="bg-warning/30 text-base-content/80">
							<tr>
								<th
									class="px-8 py-3 text-left text-sm font-bold"
									>Date</th
								>
								<th
									class="px-4 py-3 text-left text-sm font-bold"
									>Wallet</th
								>
								<th
									class="px-4 py-3 text-left text-sm font-bold"
									>Type</th
								>
								<th
									class="px-4 py-3 text-left text-sm font-bold"
									>Category</th
								>
								<th
									class="px-4 py-3 text-left text-sm font-bold"
									>Description</th
								>
								<th
									class="px-4 py-3 text-left text-sm font-bold"
									>Amount</th
								>
							</tr>
						</thead>
						<tbody class="divide-y divide-base-300">
							{#each fetched_transactions.filter((transaction) => transaction.is_archived === 0).slice(-15) as transaction (transaction.id)}
								<tr
									class="hover:bg-base-100 bg-base-200 border-b rounded-b-4xl border-b-base-content/20 transition-colors" onclick={() => window.location.hash = '#/allTransactions'}
								>
									<td
										class="px-6 py-3 text-sm text-base-content"
									>
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
											<span
												>{transaction.wallet_name}</span
											>
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
									<td
										class="px-4 py-3 text-sm text-base-content"
									>
										{transaction.category}
									</td>
									<td
										class="px-4 py-3 text-sm text-base-content"
									>
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
										<span
											class={get_type_color(
												transaction.type
											)}
										>
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
								</tr>
							{/each}
						</tbody>
					</table>
				</div>
			</div>
			<!-- Empty state -->
			{#if fetched_transactions.filter((t) => t.is_archived === 0).length === 0}
				<div class="text-center py-2">
					<i class="fas fa-file-alt text-error mb-2 text-5xl"></i>
					<p class="text-base-content text-lg">
						No transactions found
					</p>
					<p class="text-base-neutral text-sm">
						<!-- svelte-ignore a11y_missing_attribute -->
						<!-- svelte-ignore a11y_click_events_have_key_events -->
						<!-- svelte-ignore a11y_no_static_element_interactions -->
						Click
						<a
							href="#/allTransactions"
							class="hover:text-warning cursor-pointer transition-all duration-300"
							>Add Transaction</a
						> to create your first transaction.
					</p>
				</div>
			{/if}
		</div>
	</div>
</main>
