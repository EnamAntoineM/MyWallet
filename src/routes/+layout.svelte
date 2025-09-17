<script lang="ts">
	import "../app.css";
	import appIcon from "$lib/assets/icon.png?inline";
	import userPic from "$lib/assets/userPic.png?inline";
	import appLogo from "$lib/assets/logo.png?inline";
	import { logout, refresh, delete_wallet } from "$lib/index";
	import { onMount } from "svelte";
	import { goto } from "$app/navigation";
	import type { Wallet, Transaction } from "$lib/index";

	let profilePic: string = $state("");
	let fetched_wallets: Wallet[] = $state([]);
	let fetched_transactions: Transaction[] = $state([]);
	let showProfile: boolean = $state(false);

	onMount(async () => {
		refresh_wrap();
		const savedPP = localStorage.getItem("ProfilePic");
		if (savedPP) profilePic = savedPP;
	});
	async function refresh_wrap() {
		const { wallets, records } = await refresh();
		fetched_wallets = wallets;
		fetched_transactions = records;
	}
	const themes = [
		"light",
		"dark",
		"cupcake",
		"bumblebee",
		"emerald",
		"corporate",
		"synthwave",
		"retro",
		"cyberpunk",
		"valentine",
		"halloween",
		"garden",
		"forest",
		"aqua",
		"lofi",
		"pastel",
		"fantasy",
		"wireframe",
		"black",
		"luxury",
		"dracula",
		"cmyk",
		"autumn",
		"business",
		"acid",
		"lemonade",
		"night",
		"coffee",
		"winter",
		"dim",
		"nord",
		"sunset",
		"caramellatte",
		"abyss",
		"silk",
	];
	let { fName, lName, email, avatar } = JSON.parse(
		localStorage.getItem("user") ?? "{}"
	);
	let { children } = $props();
	let active: boolean = $state(true);
	let reset: boolean = $state(false);
	let deployDrawer: boolean = $state(false);
	function onToggle() {
		active = !active;
	}
	let currentTheme = $state("light");
	onMount(() => {
		if (typeof window !== "undefined") {
			const savedTheme = localStorage.getItem("app-theme") || "light";
			currentTheme = savedTheme;
			document.documentElement.setAttribute("data-theme", savedTheme);
		}
		if (
			!localStorage.getItem("isLoggedIn") ||
			!localStorage.getItem("user")
		) {
			goto("#/signUp");
		} else {
			deployDrawer = true;
		}
	});
	function handleThemeChange(theme: string) {
		currentTheme = theme;
		if (typeof window !== "undefined") {
			document.documentElement.setAttribute("data-theme", theme);
			localStorage.setItem("app-theme", theme);
		}
	}
	async function resetApplication() {
		localStorage.clear();
		for (let i = 0; i < fetched_wallets.length; i++) {
			await delete_wallet(fetched_wallets[i]);
		}
		window.location.hash = "#/signUp";
		window.location.reload();
	}
	function changeProfilePicture(event: Event) {
		const file = (event.target as HTMLInputElement).files?.[0];
		if (file) {
			const reader = new FileReader();
			reader.onload = () => {
				profilePic = reader.result as string;
				localStorage.setItem("ProfilePic", profilePic);
			};
			reader.readAsDataURL(file);
		}
	}
</script>

<svelte:head>
	<link rel="icon" href={appIcon} />
</svelte:head>

<main class="bg-base-100 text-base-content">
	<div class="drawer overflow-auto">
		<input
			id="my-drawer"
			type="checkbox"
			class="drawer-toggle bg-base-100"
			onchange={onToggle}
		/>
		<div class="drawer-content h-screen bg-base-100">
			<!-- Page content here -->
			<div
				class="navbar shadow-sm sticky z-[5000] top-0 left-0 right-0 rounded-b-sm bg-base-100"
			>
				<div class="flex-1 bg-base-100">
					<div class="flex max-w-56 bg-base-100">
						{#if deployDrawer}
							<label
								for="my-drawer"
								class="border-base-300 btn transition-all hover:shadow-lg bg-base-100 text-base-content duration-580 rounded-xl hover:bg-base-200 btn-ghost drawer-button"
							>
								<i class="fa-solid fa-bars"></i>
							</label>
						{/if}
						<a
							href={deployDrawer ? "/#" : "#/signUp"}
							class="font-bold"
						>
							<img
								alt="Application logo"
								src={appLogo}
								class="w-56 pl-4"
							/>
						</a>
					</div>
				</div>

				<div class="dropdown">
					<div tabindex="0" role="button" class="btn m-1">
						Theme
						<svg
							width="12px"
							height="12px"
							class="inline-block h-2 w-2 fill-current opacity-60"
							xmlns="http://www.w3.org/2000/svg"
							viewBox="0 0 2048 2048"
						>
							<path
								d="M1799 349l242 241-1017 1017L7 590l242-241 775 775 775-775z"
							/>
						</svg>
					</div>

					<ul
						class="dropdown-content bg-base-300 rounded-box z-1 w-52 p-2 shadow-2xl max-h-[60vh] overflow-auto"
					>
						{#each themes as t}
							<li>
								<input
									type="radio"
									name="theme-dropdown"
									class="theme-controller w-full btn btn-sm btn-ghost justify-start"
									aria-label={t.charAt(0).toUpperCase() +
										t.slice(1)}
									onchange={() => handleThemeChange(t)}
									value={t}
								/>
							</li>
						{/each}
					</ul>
				</div>
				<div class="mr-6">
					<div class="dropdown dropdown-end">
						<div
							tabindex="0"
							role="button"
							class="btn btn-ghost focus:outline-none focus:shadow-2xl focus:ring btn-circle avatar focus:ring-warning"
						>
							<div class="w-24 rounded-full">
								<img
									alt="User Profile"
									src={profilePic || userPic}
								/>
							</div>
						</div>
						<ul
							class="menu gap-y-1 menu-sm dropdown-content bg-base-100 rounded-box z-1 mt-3 w-52 p-4 shadow-sm drop-shadow-2xl"
						>
							{#if deployDrawer}
								<li>
									<button
										onclick={() => (showProfile = true)}
										class="p-4 text-lg text-base-content border border-base-300 bg-base-100 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-warning hover:shadow-xl hover:bg-base-200"
										>Profile</button
									>
								</li>
								<li>
									<button
										class="p-4 text-lg text-base-content border border-base-300 bg-base-100 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-warning hover:shadow-xl hover:bg-base-200"
										onclick={() => (
											logout(), window.location.reload()
										)}>Logout</button
									>
								</li>
							{:else}
								<li>
									<button
										class="p-4 text-lg text-base-content border border-base-300 bg-base-100 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-error hover:shadow-xl hover:bg-base-200"
										onclick={() => (reset = true)}
										>RESET!</button
									>
								</li>
							{/if}
						</ul>
					</div>
				</div>
			</div>
			{#if showProfile}
				<div class="modal modal-open">
					<div class="modal-box flex p-7 flex-col relative">
						<div class="flex flex-1 pt-3 gap-12">
							<div class="avatar">
								<div class="w-30 rounded-xl">
									<img
										alt="User profile"
										src={profilePic || userPic}
									/>
								</div>
							</div>
							<div class="text-center mt-4">
								<h2 class="text-2xl font-bold">
									{fName}
									{lName}
								</h2>
								<p class="mt-2 text-primary font-semibold">
									{email}
								</p>
							</div>
						</div>
						<input
							type="file"
							accept="image/*"
							onchange={changeProfilePicture}
							class="file-input mt-3 file-input-bordered file-input-xs w-24 rounded-2xl"
						/>
						<button
							class="btn ml-auto p-4 text-lg rounded-2xl hover:bg-error/70"
							onclick={() => (showProfile = false)}>Close</button
						>
					</div>
				</div>
			{/if}
			{#if reset}
				<div class="modal modal-open">
					<div class="modal-box">
						<h3 class="font-extrabold text-2xl text-error">
							Confirm Reset
						</h3>
						<p class="py-4">
							⚠️ This will permanently delete your account and all
							data. Are you sure you want to continue?
						</p>
						<div class="modal-action mt-0">
							<button
								class="btn btn-error"
								onclick={resetApplication}>Yes, Reset</button
							>
							<button class="btn" onclick={() => (reset = false)}
								>Cancel</button
							>
						</div>
					</div>
				</div>
			{/if}

			<div
				class="transition-all duration-400 bg-base-100"
				class:ml-[300px]={!active}
				class:ml-[0px]={active}
			>
				{@render children?.()}
			</div>
		</div>
		<div
			class="drawer-side mt-[64.6px] border-t-0 w-75 shadow-sm rounded-r-xl h-full border-r border-base-300 flex-1 flex-col"
		>
			<label for="my-drawer" aria-label="close sidebar"></label>
			<ul
				class="menu bg-base-100 text-base-content min-h-full w-80 p-4 flex-1 gap-y-4 flex-col"
			>
				<!-- Sidebar content here -->
				<li>
					<a
						class="p-5 text-lg text-base-content rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-warning hover:shadow-xl hover:bg-base-200"
						href="#/"
					>
						<i class="mr-2 fas fa-book"></i> Overview</a
					>
				</li>
				<li>
					<a
						class="p-5 text-lg text-base-content rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-warning hover:shadow-xl hover:bg-base-200"
						href="#/allWallets"
					>
						<i class="mr-2 fas fa-wallet"></i> All wallets</a
					>
				</li>
				<li>
					<a
						class="p-5 text-lg text-base-content rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-warning hover:shadow-xl hover:bg-base-200"
						href="#/allTransactions"
					>
						<i class="mr-2 fas fa-exchange-alt"></i> All Transactions</a
					>
				</li>
				<li>
					<a
						class="p-5 text-lg text-error/70 rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-error hover:shadow-xl hover:bg-base-200"
						href="#/recyclebin"
					>
						<i class="mr-2 fas fa-trash"></i> Recycle Bin</a
					>
				</li>
				<li class="mt-auto"></li>
				<div class="mb-16">
					<li>
						<a
							class="p-5 shadow-sm text-lg text-base-content rounded-2xl transition-all duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-warning hover:shadow-xl hover:bg-base-200"
							href="#/license"
						>
							<i class="mr-2 fas fa-scale-balanced"></i> License</a
						>
					</li>
					<li>
						<a
							class="p-5 shadow-sm mt-4 text-lg text-base-content rounded-2xl transition-all duration-580 drop-shadow-3xl border border-base-300 bg-base-100 font-bold max-w-64 ml-2 hover:text-warning hover:shadow-xl hover:bg-base-200"
							href="#/aboutUs"
						>
							<i class="mr-2 fas fa-info-circle"></i> About Us</a
						>
					</li>
				</div>
			</ul>
		</div>
	</div>
</main>

<style>
	@reference "tailwindcss";
</style>
