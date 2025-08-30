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

<main class="bg-gray-50">
	<div class="drawer overflow-auto">
		<input id="my-drawer" type="checkbox" class="drawer-toggle bg-gray-50" onchange={onToggle}/>
		<div class="drawer-content h-screen bg-gray-50">
			<!-- Page content here -->
			<div class="navbar shadow-sm fixed z-[5000] top-0 left-0 right-0 rounded-b-sm bg-gray-50">
				<div class="flex-1 bg-gray-50">
					<div class="flex max-w-56 bg-gray-50">
						<label for="my-drawer" class=" border-gray-300 btn transition-all hover:shadow-lg bg-gray-50 text-black duration-580 rounded-xl hover:bg-gray-300 btn-ghost drawer-button">
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
						<ul class="menu gap-y-1 menu-sm dropdown-content bg-gray-50 rounded-box z-1 mt-3 w-52 p-4 shadow-sm drop-shadow-2xl">
							<li><button class = "p-4 text-lg text-black border border-gray-200 bg-gray-50 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300">Profile</button></li>
							<li><button class = "p-4 text-lg text-black border border-gray-200 bg-gray-50 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300">Settings</button></li>
							<li><button class = "p-4 text-lg text-black border border-gray-200 bg-gray-50 font-bold rounded-2xl shadow-sm transition-all duration-580 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300">Logout</button></li>
						</ul>
					</div>
				</div>
			</div>

			<div class="transition-all duration-400 p-2 overflow-auto mt-11 bg-gray-50"
				class:ml-[320px]={!active}
  				class:ml-[0px]={active}
			>
				{@render children?.()}
			</div>

		</div>
		<div class="drawer-side mt-[64.6px] border-t-0 w-75 shadow-sm rounded-r-xl h-full border-r border-r-gray-200  flex-1 flex-col">
			<label for="my-drawer" aria-label="close sidebar"></label>
			<ul class="menu bg-gray-50 text-base-content min-h-full w-80 p-4 flex-1 gap-y-4 flex-col">
				<!-- Sidebar content here -->
				<li><a class="p-5 text-lg text-black rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/">                <i class="mr-2 fas fa-book"></i>         Overview</a>         </li>
				<li><a class="p-5 text-lg text-black rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/allWallets">      <i class="mr-2 fas fa-wallet"></i>       All wallets</a>      </li>
				<li><a class="p-5 text-lg text-black rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/allTransactions"> <i class="mr-2 fas fa-exchange-alt"></i> All Transactions</a> </li>
				<li><a class="p-5 text-lg text-black rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/statistics">      <i class="mr-2 fas fa-chart-line"></i>   Statistics</a>       </li>
				<li><a class="p-5 text-lg text-red-800 rounded-2xl transition-all shadow-sm duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/docs">      <i class="mr-2 fas fa-circle-exclamation"></i>   DOCUMENTATION</a>       </li>
				<li class="mt-auto"></li>
				<div class="mb-16">
					<li><a class="p-5 shadow-sm text-lg text-black rounded-2xl transition-all duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/license">     <i class="mr-2 fas fa-scale-balanced"></i>      License</a></li>
					<li><a class="p-5 shadow-sm mt-4 text-lg text-black rounded-2xl transition-all duration-580 drop-shadow-3xl border border-gray-200 bg-gray-50 font-bold max-w-64 ml-2 hover:text-yellow-600 hover:shadow-xl hover:bg-gray-300" href="#/aboutUs">     <i class="mr-2 fas fa-info-circle"></i>         About Us</a></li>
				</div>
			</ul>
		</div>
	</div>
</main>

<style>
  @reference "tailwindcss";
</style>

