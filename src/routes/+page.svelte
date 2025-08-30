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
		color: "#fffff",
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
		if (fetched_transactions.length === 0)
			error_popup = "No transactions found";
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
<main>
    <p class = "font-extrabold justify-center text-black border mt-6 rounded-2xl text-7xl p-64">This is the overview page</p>
</main>
