<script lang="ts">
	import type { Wallet, Transaction } from '$lib';
	import {
		get_wallets,
		delete_wallet,
		restore_wallet,
		refresh,
	} from '$lib';
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
	let empty: boolean = $state(true);
	let success_popup: string = $state("");
	let error_popup: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);

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

	async function delete_wallet_wrap(to_delete: Wallet) {
		({ success_popup, error_popup } = await delete_wallet(to_delete));
		await refresh_wrap();
		fade_alert();
	}
</script>

<main class="pt-6 bg-gradient-to-br from-base-200 to-base-content/10 min-h-screen">
	<div>
		{#if success_popup}<div class="alert alert-success">
				{success_popup}
			</div>{/if}
		{#if error_popup}<div class="alert alert-error">{error_popup}</div>{/if}
	</div>
	<h1 class="font-extrabold text-4xl ml-6 p-6 text-base-content">
		Recycle Bin
	</h1>
	{#if fetched_wallets.some((delWallets) => delWallets.is_active === false)}
		<div>
			<div
				class="grid md:grid-cols-1 md:grid-rows-8 lg:grid-cols-2 lg:grid-rows-4 xl:grid-rows-4 xl:grid-cols-2 2xl:grid-rows-3 2xl:grid-cols-3 gap-4 pt-7 ml-16 justify-center"
			>
				{#each fetched_wallets as delWallet}
					{#if !delWallet.is_active}
						<div
							class="card !bg-primary/60 text-primary-content w-96"
						>
							<div class="card-body">
								<div class="flex">
									<i class="fa-solid fa-magnet text-xl"></i>
									<h2
										class="card-title ml-auto mr-6 font-extrabold text-2xl"
									>
										{delWallet.name}
									</h2>
								</div>
								<div class="flex mt-4 text-lg mb-2">
									<p class="font-bold">Balance:</p>
									<div class="ml-auto flex font-semibold mr-4 gap-x-2">
										<p>{delWallet.currency}</p>
										<p>{delWallet.balance}</p>
									</div>
								</div>
								<div
									class="card-actions justify-end mt-2 gap-x-2"
								>
									<button
										class="btn rounded-2xl hover:bg-error hover:border-error"
										onclick={() => delete_wallet_wrap(delWallet)}>Delete</button
									>
									<button
										class="btn rounded-2xl hover:bg-success hover:border-success"
										onclick={() =>
											restore_wallet_wrap(delWallet)}
										>Restore</button
									>
								</div>
							</div>
						</div>
					{/if}
				{/each}
			</div>
		</div>
	{:else}
		<div class="text-center py-2">
			<i class="fas fa-file-alt text-error mb-2 text-5xl"></i>
			<p class="text-base-content text-lg">No deleted wallets</p>
			<p class="text-base-neutral text-sm">
				Your deleted wallets will appear here.
			</p>
		</div>
	{/if}
</main>
