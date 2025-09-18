<script lang="ts">
	import type { Wallet, Transaction } from "$lib/index";
	import {
		create_wallet,
		update_wallet,
		trash_wallet,
		refresh,
	} from "$lib/index";
	import { onMount } from "svelte";

	let a_wallet: Wallet = $state({
		id: 0,
		name: "",
		currency: "",
		source: "",
		initial_amount: 0,
		balance: 0,
		color: "#ffffff",
		created_at: "",
		updated_at: "",
		is_active: true,
	});

	const accountTypes = [
		"Cash",
		"Credit card",
		"Saving account",
		"Insurance",
		"Investment",
		"Loan",
		"Mortgage",
	];
	let selectedType: string = $state("Select a type");
	const currencies = [
		"USD", // US Dollar
		"GHS", // Ghana
		"FCFA",
		"NGN",
		"AED",
		"AFN",
		"ALL",
		"AMD",
		"EUR", // Euro
		"JPY", // Japanese Yen
		"GBP", // British Pound
		"AUD", // Australian Dollar
		"CAD", // Canadian Dollar
		"CHF", // Swiss Franc
		"CNY", // Chinese Yuan
		"HKD", // Hong Kong Dollar
		"NZD", // New Zealand Dollar
		"SEK", // Swedish Krona
		"NOK", // Norwegian Krone
		"DKK", // Danish Krone
		"SGD", // Singapore Dollar
		"INR", // Indian Rupee
		"BRL", // Brazilian Real
		"ZAR", // South African Rand
		"RUB", // Russian Ruble
	];

	let selectedCurrency: string = $state("Select a currency");
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);
	let show_create_form: boolean = $state(false);
	let is_editing: boolean = $state(false); // Flag to track if we're editing

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

	async function create_wallet_wrap(to_create: Wallet) {
		to_create.balance = to_create.initial_amount;
		({ success_popup, error_popup } = await create_wallet(to_create));
		await refresh_wrap();
		fade_alert();
	}

	async function update_wallet_wrap(to_update: Wallet) {
		({ success_popup, error_popup } = await update_wallet(to_update));
		await refresh_wrap();
		fade_alert();
	}

	async function trash_wallet_wrap(to_trash: Wallet) {
		({ success_popup, error_popup } = await trash_wallet(to_trash));
		await refresh_wrap();
		fade_alert();
	}

	// Simplified edit function - just populate a_wallet with the wallet data
	function start_edit_wallet(wallet: Wallet) {
		a_wallet = { ...wallet };
		selectedCurrency = wallet.currency;
		selectedType = wallet.source;
	}

	// Reset form and exit edit mode
	function reset_form() {
		a_wallet = {
			id: 0,
			name: "",
			currency: "",
			source: "",
			initial_amount: 0,
			balance: 0,
			color: "#ffffff",
			created_at: "",
			updated_at: "",
			is_active: true,
		};
		selectedCurrency = "Select a currency";
		selectedType = "Select a type";
	}
</script>

<main
	class="min-h-screen p-12 w-full space-y-6 bg-gradient-to-br from-base-200 to-base-content/10"
>
	<!-- Feedback -->
	{#if success_popup}
		<div class="alert alert-success">{success_popup}</div>
	{/if}
	{#if error_popup}
		<div class="alert alert-error">{error_popup}</div>
	{/if}

	<div class="flex justify-between items-center">
		<h1 class="text-4xl font-bold text-base-content">Wallet Management</h1>
		<button
			onclick={() => {
				show_create_form = true;
				reset_form();
			}}
			class="flex items-center cursor-pointer gap-2 bg-warning text-base-100 px-4 py-2 rounded-lg hover:drop-shadow-xl transition-colors"
		>
			<i class="fas fa-plus"></i>
			Add wallet
		</button>
	</div>
	<!-- Create/Edit wallet -->
	<section class="card">
		{#if show_create_form || is_editing}
			<div class="mb-6 bg-base-100 p-8 rounded-lg border shadow-sm">
				<h3 class="text-lg font-semibold mb-4">
					{show_create_form ? "Create New" : "Update"} Wallet
				</h3>
				<div class="grid grid-cols-1 md:grid-cols-2 gap-4">
					<div>
						<p
							class="block text-sm bg-base-100 font-medium text-base-content mb-1"
						>
							Name
						</p>
						<input
							type="text"
							bind:value={a_wallet.name}
							placeholder="Enter name"
							class="w-full px-3 py-2 border-2 border-neutral/20 rounded-md focus:outline-none focus:ring-1 focus:ring-warning"
						/>
					</div>
					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Currency
						</p>
						<div class="dropdown w-full">
							<div tabindex="0" role="button" class="btn w-full">
								<span class="mr-auto">{selectedCurrency}</span>
							</div>
							<!-- svelte-ignore a11y_no_noninteractive_tabindex -->
							<ul
								tabindex="0"
								class="dropdown-content menu bg-base-100 rounded-box z-1 w-full p-2 shadow-sm"
							>
								{#each currencies as currency}
									<!-- svelte-ignore a11y_missing_attribute -->
									<!-- svelte-ignore a11y_click_events_have_key_events -->
									<!-- svelte-ignore a11y_no_static_element_interactions -->
									<li>
										<a
											onclick={() => (
												(a_wallet.currency = currency),
												(selectedCurrency = currency),
												(
													document.activeElement as HTMLElement
												)?.blur()
											)}
										>
											{currency}</a
										>
									</li>
								{/each}
							</ul>
						</div>
					</div>

					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Account Type
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
								{#each accountTypes as accType}
									<!-- svelte-ignore a11y_missing_attribute -->
									<!-- svelte-ignore a11y_click_events_have_key_events -->
									<!-- svelte-ignore a11y_no_static_element_interactions -->
									<li>
										<a
											onclick={() => (
												(a_wallet.source = accType),
												(selectedType = accType),
												(
													document.activeElement as HTMLElement
												)?.blur()
											)}
										>
											{accType}</a
										>
									</li>
								{/each}
							</ul>
						</div>
					</div>
					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Initial amount
						</p>
						<div class="flex-1 flex gap-x-1">
							<div
								class="badge badge-soft h-full badge-warning font-bold p-3"
							>
								{a_wallet.currency}
							</div>
							<input
								type="number"
								bind:value={a_wallet.initial_amount}
								placeholder={"Enter the amount"}
								class="w-full px-3 py-2 border-2 border-neutral/20 rounded-md focus:outline-none focus:ring-1 focus:ring-warning"
							/>
						</div>
					</div>
					{#if is_editing}
						<div>
							<p
								class="block text-sm font-medium text-base-content mb-1"
							>
								Balance
							</p>
							<div class="flex-1 flex gap-x-1">
								<div
									class="badge badge-soft h-full badge-warning font-bold p-3"
								>
									{a_wallet.currency}
								</div>
								<input
									type="number"
									bind:value={a_wallet.balance}
									placeholder={"Enter the amount"}
									class="w-full px-3 py-2 border-2 border-neutral/20 rounded-md focus:outline-none focus:ring-1 focus:ring-warning"
								/>
							</div>
						</div>
					{/if}
					<div>
						<p
							class="block text-sm font-medium text-base-content mb-1"
						>
							Wallet color
						</p>
						<input
							class="input input-bordered w-full"
							type="color"
							placeholder="Pick a color"
							bind:value={a_wallet.color}
						/>
					</div>
				</div>

				<div class="flex gap-2 justify-end mt-4">
					<button
						class="btn btn-primary"
						onclick={() => {
							(is_editing
								? update_wallet_wrap(a_wallet)
								: create_wallet_wrap(a_wallet),
							reset_form()),
								(is_editing = false),
								(show_create_form = false);
						}}
					>
						{is_editing ? "Update Wallet" : "Create Wallet"}
					</button>

					<button
						class="btn hover:bg-error"
						onclick={() => {
							reset_form(),
								(is_editing = false),
								(show_create_form = false);
						}}
					>
						Cancel
					</button>
				</div>
			</div>
		{/if}
	</section>

	<!-- Wallets list with Glass Morphism Design -->
	<section class="card">
		{#if fetched_wallets.length === 0}
			<div class="text-center py-2 w-full">
				<i class="fas fa-file-alt text-error mb-2 text-5xl"></i>
				<p class="text-base-content text-lg">No wallets found</p>
				<p class="text-base-neutral text-sm">
					<!-- svelte-ignore a11y_missing_attribute -->
					<!-- svelte-ignore a11y_click_events_have_key_events -->
					<!-- svelte-ignore a11y_no_static_element_interactions -->
					Click
					<a
						onclick={() => {
							show_create_form = true;
							reset_form();
						}}
						class="hover:text-warning cursor-pointer transition-all duration-300"
					>
						Add Wallet</a
					> to create your first wallet.
				</p>
			</div>
		{:else}
			<div class="grid grid-cols-1 md:grid-cols-2 xl:grid-cols-3 gap-10">
				{#each fetched_wallets as w (w.id)}
					{#if w.is_active}
						<!-- Glass Morphism Card -->
						<div
							class="relative p-6 rounded-2xl backdrop-blur-sm bg-base-200/70 border border-base-300 shadow-xl hover:shadow-2xl transition-all duration-300 hover:bg-base-100"
						>
							<!-- Header with icon and wallet info -->
							<div class="flex items-center gap-3 mb-4">
								<div
									class="w-12 h-12 rounded-full flex items-center justify-center backdrop-blur-sm border border-base-300"
									style="background-color: {w.color}20;"
								>
									<!-- Wallet Icon -->
									<svg
										class="w-6 h-6"
										style="color: {w.color};"
										fill="none"
										stroke="currentColor"
										viewBox="0 0 24 24"
									>
										<path
											stroke-linecap="round"
											stroke-linejoin="round"
											stroke-width="2"
											d="M3 10h18M7 15h1m4 0h1m-7 4h12a3 3 0 003-3V8a3 3 0 00-3-3H6a3 3 0 00-3 3v8a3 3 0 003 3z"
										/>
									</svg>
								</div>
								<div>
									<h4
										class="font-bold text-base-content text-lg"
									>
										{w.name}
									</h4>
									<p class="text-sm text-base-content/80">
										{w.source} • {w.currency}
									</p>
								</div>
							</div>

							<!-- Balance Display -->
							<div class="ml-4 mb-4">
								<div class="flex items-baseline gap-1">
									<span
										class="text-3xl font-bold text-base-content"
									>
										{w.balance + "  " + w.currency}
									</span>
								</div>
								<div class="flex items-center gap-2 mt-2">
									<!-- Trending up/down icon -->
									<svg
										class="w-4 h-4 {w.balance >=
										w.initial_amount
											? 'text-success'
											: 'text-error'}"
										fill="none"
										stroke="currentColor"
										viewBox="0 0 24 24"
									>
										{#if w.balance >= w.initial_amount}
											<path
												stroke-linecap="round"
												stroke-linejoin="round"
												stroke-width="2"
												d="M13 7h8m0 0v8m0-8l-8 8-4-4-6 6"
											/>
										{:else}
											<path
												stroke-linecap="round"
												stroke-linejoin="round"
												stroke-width="2"
												d="M13 17h8m0 0V9m0 8l-8-8-4 4-6-6"
											/>
										{/if}
									</svg>
									<span
										class="text-sm {w.balance >=
										w.initial_amount
											? 'text-success'
											: 'text-error'}"
									>
										{w.balance >= w.initial_amount
											? "+"
											: ""}
										{w.balance - w.initial_amount}
										{w.currency + " "} from initial
									</span>
								</div>
							</div>

							<!-- Action Buttons -->
							<div class="flex gap-2">
								<button
									class="flex-1 py-2 px-3 rounded-lg text-base-100 font-medium transition-all duration-200 hover:scale-105 active:scale-95"
									style="background-color: {w.color};"
									onclick={() => {
										start_edit_wallet(w);
										is_editing = true;
									}}
								>
									Edit
								</button>
								<button
									class="py-2 px-3 bg-base-300 backdrop-blur-sm text-base-content rounded-lg hover:bg-error/70 hover:text-error transition-all duration-200"
									onclick={() => trash_wallet_wrap(w)}
								>
									<!-- Delete icon -->
									<svg
										class="w-4 h-4"
										fill="none"
										stroke="currentColor"
										viewBox="0 0 24 24"
									>
										<path
											stroke-linecap="round"
											stroke-linejoin="round"
											stroke-width="2"
											d="M19 7l-.867 12.142A2 2 0 0116.138 21H7.862a2 2 0 01-1.995-1.858L5 7m5 4v6m4-6v6m1-10V4a1 1 0 00-1-1h-4a1 1 0 00-1 1v3M4 7h16"
										/>
									</svg>
								</button>
							</div>

							<!-- Additional Info -->
							<div
								class="mt-4 pt-3 border-t border-base-content/10"
							>
								<div
									class="flex justify-between text-xs text-base-content/80"
								>
									<span>
										Initial amount:
										{w.currency === "USD"
											? "$"
											: ""}{w.initial_amount.toLocaleString()}{w.currency !==
										"USD"
											? ` ${w.currency}`
											: ""}
									</span>
								</div>
							</div>
						</div>
					{/if}
				{/each}
			</div>
		{/if}
	</section>
</main>
