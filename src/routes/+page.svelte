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
	import WalletModal from './WalletModal.svelte';

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
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);
	let editingWallet: Wallet | null = $state(null);
	let showWalletModal = $state(false);


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
	function addWallet() {
		editingWallet = {
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
		showWalletModal = true;
	}
	function editWallet(wallet: Wallet) {
		editingWallet = { ...wallet };
		showWalletModal = true;
	}

	function handleWalletSave(updatedWallet: Wallet) {
		if (updatedWallet.id === 0) {
			create_wallet_wrap(updatedWallet);
		} else {
			update_wallet_wrap(updatedWallet);
		}
		showWalletModal = false;
	}

	function showSuccess(message: string) {
		success_popup = message;
		fade_alert();
	}

	function showError(message: string) {
		error_popup = message;
		fade_alert();
	}
</script>
<main class="bg-base-100 text-base-content">
	<div class="container">
		<section class="wallet-section">
			<div class="wallet-card-header">
				<p>Wallets</p>
				<button class="Add" on:click={addWallet}>Add Wallet <i class="fas fa-plus"></i> </button>
			</div>
			<div class="cards">
				{#each fetched_wallets as wallet (wallet.id)}
				<div class="wallet-card" style="border-left-color: {wallet.color};">
					<div class="card-header">
						<div class="card-title">
							<p>{wallet.name}</p><i class="fas fa-user" style="color: {wallet.color}"></i>
						</div>
					</div>
					<div class="card-value">${wallet.balance} {wallet.currency}</div>
					<div class="currency"><i class="fas fa-dollar-sign"> {wallet.source} </i> </div>
						<button class="edit-button" on:click = {() => editWallet(wallet)}>Edit</button>
					</div>
				{/each}
			</div>
		</section>
		<!-- Recent Transactions Section -->
		<section class="bg-white rounded-lg shadow-md p-6">
			<div class="flex justify-between items-center mb-6">
				<h2 class="text-2xl font-bold text-gray-800">Recent Transactions</h2>
				<a href="/allTransactions" class="text-blue-600 hover:text-blue-800 text-sm font-medium">
					View all transactions →
				</a>
			</div>
			
			{#if fetched_transactions.length > 0}
				<div class="overflow-x-auto">
					<table class="w-full">
						<thead>
							<tr class="border-b border-gray-200">
								<th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Description</th>
								<th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Wallet</th>
								<th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Category</th>
								<th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Amount</th>
								<th class="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Date</th>
							</tr>
						</thead>
						<tbody class="divide-y divide-gray-200">
							{#each fetched_transactions.slice(0, 5) as transaction (transaction.id)}
							<tr class="hover:bg-gray-50 transition-colors">
								<td class="px-6 py-4 whitespace-nowrap text-sm font-medium text-gray-900">
									{transaction.description || 'No description'}
								</td>
								<td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
									{transaction.wallet_name}
								</td>
								<td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
									<span class="inline-flex items-center px-2.5 py-0.5 rounded-full text-xs font-medium bg-blue-100 text-blue-800">
										{transaction.category}
									</span>
								</td>
								<td class="px-6 py-4 whitespace-nowrap text-sm font-medium" class:text-green-600={transaction.type === 'income'} class:text-red-600={transaction.type !== 'income'}>
									{transaction.type === 'income' ? '+' : '-'}${Math.abs(transaction.amount)}
								</td>
								<td class="px-6 py-4 whitespace-nowrap text-sm text-gray-500">
									{new Date(transaction.recorded_at).toLocaleDateString()}
								</td>
							</tr>
							{/each}
						</tbody>
					</table>
				</div>
			{:else}
				<div class="text-center py-12">
					<div class="text-gray-400 text-6xl mb-4">
						<i class="fas fa-receipt"></i>
					</div>
					<p class="text-gray-500 text-lg">No transactions yet</p>
					<p class="text-gray-400 text-sm mt-2">Your transactions will appear here</p>
				</div>
			{/if}
		</section>
		
	</div>

	<WalletModal
		wallet={editingWallet}
		isOpen={showWalletModal}
		onSave={handleWalletSave}
		onClose={() => showWalletModal = false}
	/>
	{#if success_popup}
		<div class="toast toast-top toast-center">
			<div class="alert alert-success">
				<span>{success_popup}</span>
			</div>
		</div>
	{/if}

	{#if error_popup}
		<div class="toast toast-top toast-center">
			<div class="alert alert-error">
				<span>{error_popup}</span>
			</div>
		</div>
	{/if}	

</main>
<style>
	
.wallet-card-header{
    font-size: larger;
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding-bottom: 20px;
}

.cards{
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 20px;
}
.wallet-section {
    background: white;
    border-radius: 12px;
    padding: 20px;
    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
    margin-bottom: 30px;
}

.wallet-card {
    background: white;
    border-radius: 12px;
    padding: 20px;
    box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
    border-left: 4px solid #6cbbf2;
    min-width: 280px;}

.card-header {
    justify-content: space-between;
    align-items: center;
    margin-bottom: 15px;
}

.card-title{
    display: flex;
    justify-content: space-between
}

.fa-user {
    color: #6a11cb;
}

.card-value{
    display: flex;
    font-size: 28px;
    font-weight: 700;
    color: rgba(0,0,0,0.7);
    padding-top: 15px 0;
    margin-bottom: 10px;
}

.currency{
    color: #7f8c8d;
    font-size: 14px;
    margin-bottom: 15px;
}


.edit-button {
    background: #2575fc;
    color: white;
    border: none;
    padding: 8px 16px;
    border-radius: 5px;
    cursor: pointer;
    margin-top: 10px;
}

.edit-button:hover{
    background-color: darkblue;
}

.Add {
    background-color: green;
    color: white;
    border: none;
    padding: 10px 20px;
    border-radius: 5px;
    cursor: pointer;
}

.transactions-section {
    background: white;
    border-radius: 12px;
    padding: 24px;
    box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
    margin-bottom: 30px;
}

.table-container {
    overflow-x: auto;
    border-radius: 8px;
    border: 1px solid #e5e7eb;
}

table {
    width: 100%;
    border-collapse: collapse;
    background: white;
}

th {
    background-color: #f9fafb;
    padding: 12px 16px;
    text-align: left;
    font-weight: 600;
    color: #374151;
    font-size: 0.75rem;
    text-transform: uppercase;
    letter-spacing: 0.05em;
    border-bottom: 1px solid #e5e7eb;
}

td {
    padding: 16px;
    border-bottom: 1px solid #f3f4f6;
    font-size: 0.875rem;
}

tr:last-child td {
    border-bottom: none;
}

tr:hover {
    background-color: #f9fafb;
}

.badge {
    display: inline-flex;
    align-items: center;
    padding: 0.25rem 0.75rem;
    border-radius: 9999px;
    font-size: 0.75rem;
    font-weight: 500;
}

.badge-income {
    background-color: #dcfce7;
    color: #166534;
}

.badge-expense {
    background-color: #fee2e2;
    color: #991b1b;
}

.amount-income {
    color: #059669;
    font-weight: 600;
}

.amount-expense {
    color: #dc2626;
    font-weight: 600;
}

.empty-state {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding: 3rem 1rem;
    color: #6b7280;
}

.empty-state-icon {
    font-size: 3rem;
    margin-bottom: 1rem;
    opacity: 0.5;
}


</style>