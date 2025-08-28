<!--In this file, we are creating the base layout for our application
It will be displayed across all the pages of our application.
This includes:
	- the navigation bar
			=> the logo
			=> the user profile

	- the left panel
			=> the overview
			=> All wallets
			=> All transactions
			=> Statistics (if possible)
			=> About Us
			=> License
-->
<script lang="ts">
	import '../app.css';
	import appIcon from '$lib/assets/icon.png';
	import userPic from '$lib/assets/userPic.png';
	import appLogo from '$lib/assets/logo.png';

	let { children } = $props();
	let marginleft: number = 5;
	let active: boolean = $state(true);
	function onToggle() {
		active = !active;
	}
</script>

<svelte:head>
	<link rel="icon" href="{appIcon}" />
</svelte:head>

<main>
	<div class="drawer overflow-auto">
		<input id="my-drawer" type="checkbox" class="drawer-toggle" onchange={onToggle}/>
		<div class="drawer-content h-screen">
			<!-- Page content here -->
			<div class="navbar bg-base-100 shadow-md fixed rounded-b-sm">
				<div class="flex-1">
					<div class="flex max-w-56">
						<label for="my-drawer" class="btn transition-all duration-580 rounded-xl hover:bg-yellow-500 btn-ghost drawer-button">
							<i class = "fa-solid fa-bars"></i>
						</label>
						<a href="#/" class="font-bold">
							<img alt="Application logo" src={appLogo} class="w-56 pl-4">
						</a>
					</div>
				</div>
				<div class="mr-6">
					<div class="dropdown dropdown-end">
						<div tabindex="0" role="button" class="btn btn-ghost focus:outline-none focus:shadow-2xl focus:ring btn-circle avatar focus:ring-yellow-500">
							<div class="w-24 rounded-full">
								<img alt="User Profile" src={userPic} />
							</div>
						</div>
						<ul class="menu menu-sm dropdown-content bg-base-100 rounded-box z-1 mt-3 w-52 p-2 shadow-sm drop-shadow-2xl">
							<li><button class = "p-4 text-lg text-white rounded-2xl drop-shadow-2xl transition-all duration-580 hover:text-yellow-500">Profile</button></li>
							<li><button class = "p-4 text-lg text-white rounded-2xl drop-shadow-2xl transition-all duration-580 hover:text-yellow-500">Settings</button></li>
							<li><button class = "p-4 text-lg text-white rounded-2xl drop-shadow-2xl transition-all duration-580 hover:text-yellow-500">Logout</button></li>
						</ul>
					</div>
				</div>
			</div>

			<div class="transition-all duration-400 p-2 overflow-auto mt-11"
				class:ml-[320px]={!active}
  				class:ml-[5px]={active}
			>
				{@render children?.()}
			</div>

		</div>
		<div class="drawer-side top-16 w-75 rounded-r-xl h-full shadow-md drop-shadow-2xl flex-1 flex-col">
			<label for="my-drawer" aria-label="close sidebar"></label>
			<ul class="menu bg-base-200 text-base-content min-h-full w-80 p-4 flex-1 gap-2 flex-col">
				<!-- Sidebar content here -->
				<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/">                <i class="mr-2 fas fa-book"></i>         Overview</a>         </li>
				<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/allWallets">      <i class="mr-2 fas fa-wallet"></i>       All wallets</a>      </li>
				<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/allTransactions"> <i class="mr-2 fas fa-exchange-alt"></i> All Transactions</a> </li>
				<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/statistics">      <i class="mr-2 fas fa-chart-line"></i>   Statistics</a>       </li>
				<li class="mt-auto"></li>
				<div class="mb-16">
					<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/license">     <i class="mr-2 fas fa-scale-balanced"></i>      License</a></li>
					<li><a class="p-6 text-lg rounded-xl transition-all duration-580 drop-shadow-3xl max-w-64 ml-2 hover:text-yellow-500" href="#/aboutUs">     <i class="mr-2 fas fa-info-circle"></i>         About Us</a></li>
				</div>
			</ul>
		</div>
	</div>
</main>

<style>
  @reference "tailwindcss";
</style>

