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

	let a_record: Transaction;
	let a_wallet: Wallet;
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);

	onMount(async () => refresh_wrap());

	a_wallet = {
		id: 0,
		name: "Frontend wallet",
		currency: "USD",
		source: "MOMO",
		initial_amount: 1000,
		balance: 800,
		color: "#ffffff",
		created_at: "",
		updated_at: "",
		is_active: true,
	};

	a_record = {
		id: 0,
		wallet_name: "Frontend wallet",
		type: "income",
		category: "transportation",
		description: "just to try",
		amount: 200,
		related_wallet_name: "",
		created_at: "",
		updated_at: "",
		is_archived: false,
	};

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
		({ success_popup, error_popup } = await create_wallet(to_create));
		await refresh_wrap();
		fade_alert();
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

	async function update_transaction_wrap(to_update: Transaction) {
		({ success_popup, error_popup } = await update_record(to_update));
		await refresh_wrap();
		fade_alert();
	}

	async function update_transfer_wrap(to_update: Transaction) {
		({ success_popup, error_popup } = await update_transfer(to_update));
		await refresh_wrap();
		fade_alert();
	}

	async function update_wallet_wrap(to_update: Wallet) {
		({ success_popup, error_popup } = await update_wallet(to_update));
		await refresh_wrap();
		fade_alert();
	}

	async function restore_wallet_wrap(to_restore: Wallet) {
		({ success_popup, error_popup } = await restore_wallet(to_restore));
		await refresh_wrap();
		fade_alert();
	}

	async function get_wallets_wrap() {
		fetched_wallets = await get_wallets();
		if (fetched_wallets.length === 0) error_popup = "No wallets found";
	}

	async function get_records_wrap() {
		fetched_transactions = await get_records();
		if (fetched_transactions.length === 0) error_popup = "No transactions found";
	}

	async function trash_wallet_wrap(to_trash: Wallet) {
		({ success_popup, error_popup } = await trash_wallet(to_trash));
		await refresh_wrap();
		fade_alert();
	}

	async function delete_wallet_wrap(to_delete: Wallet) {
		({ success_popup, error_popup } = await delete_wallet(to_delete));
		await refresh_wrap();
		fade_alert();
	}

	async function delete_record_wrap(to_delete: Transaction) {
		({ success_popup, error_popup } = await delete_record(to_delete));
		await refresh_wrap();
		fade_alert();
	}
</script>

<main class="min-h-screen p-6 max-w-3xl mx-auto space-y-6 bg-base-100 text-base-content">
	<!-- Feedback -->
	{#if success_popup}
		<div class="alert alert-success">{success_popup}</div>
	{/if}
	{#if error_popup}
		<div class="alert alert-error">{error_popup}</div>
	{/if}

	<!-- Quick controls -->
	<div class="flex gap-2">
		<button class="btn btn-outline" onclick={refresh_wrap}>Refresh</button>
		<button class="btn" onclick={get_wallets_wrap}>Load wallets only</button>
	</div>

	<!-- Create wallet -->
	<section class="card p-4">
		<h2 class="text-lg font-extrabold">Add wallet (test)</h2>

		<div class="grid grid-cols-1 md:grid-cols-2 gap-3">
			<input class="input input-bordered" placeholder="Name" bind:value={a_wallet.name} />
			<input class="input input-bordered" placeholder="Currency" bind:value={a_wallet.currency} />
			<input class="input input-bordered" placeholder="Source" bind:value={a_wallet.source} />
			<input class="input input-bordered" type="number" placeholder="Initial amount" bind:value={a_wallet.initial_amount} />
			<input class="input input-bordered" type="number" placeholder="Balance" bind:value={a_wallet.balance} />
			<input class="input input-bordered" type="text" placeholder="Color (#hex)" bind:value={a_wallet.color} />
		</div>

		<div class="flex gap-2 mt-3">
			<button class="btn btn-primary" onclick={() => create_wallet_wrap(a_wallet)}>
				Create wallet
			</button>
			<button class="btn btn-ghost" onclick={() => { 
				a_wallet = { ...a_wallet, name: "", initial_amount: 0, balance: 0, color: "#7c3aed" }; 
			}}>
				Reset fields
			</button>
		</div>
	</section>

	<!-- Wallets list -->
	<section class="card p-4">
		<div class="flex items-center justify-between">
			<h2 class="text-lg font-semibold">Wallets</h2>
			<span class="badge">{fetched_wallets.length}</span>
		</div>

		{#if fetched_wallets.length === 0}
			<p class="opacity-70">No wallets loaded. Click <b>Refresh</b> or <b>Load wallets only</b>.</p>
		{:else}
			<ul class="space-y-2">
				{#each fetched_wallets as w (w.id)}
					<li class="flex items-center justify-between p-2 rounded-lg bg-base-200">
						<div class="flex items-center gap-3">
							<div class="w-4 h-4 rounded" style="background-color: {w.color}"></div>
							<div class="flex flex-col">
								<span class="font-medium">{w.name}</span>
								<span class="text-xs opacity-70">{w.currency} • balance: {w.balance}</span>
							</div>
						</div>
						<div class="flex gap-2">
							<button class="btn btn-error btn-sm" onclick={() => delete_wallet_wrap(w)}>
								Delete (permanent)
							</button>
							<button class="btn btn-warning btn-sm" onclick={() => trash_wallet_wrap(w)}>
								Trash (soft)
							</button>
						</div>
					</li>
				{/each}
			</ul>
		{/if}
	</section>
</main>
