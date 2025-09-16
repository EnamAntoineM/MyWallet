<script lang="ts">
	import { signUp, login } from "$lib/index";
	import { goto } from "$app/navigation";
	import { onMount } from "svelte";

    let showlogin: boolean = $state(true);
    onMount(() => (!localStorage.getItem("isLoggedIn") && localStorage.getItem("user")) ? showlogin=true : showlogin = false)

	let isSignUp = $state(false);
	let isLoading = $state(false);
	let licenseName = "MIT License";
	let copyrightYear = "2025";
	let companyName = "Enam A.-M. KODJOH-KPAKPASSOU";

	let licenseText = `
  Permission is hereby granted to any person obtaining a copy of this software and associated documentation files to use this software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.`;

	// Sign up fields
	let fName = $state("");
	let lName = $state("");
	let email = $state("");
	let password = $state("");
	let confirmPassword = $state("");

	// Sign in fields
	let signInEmail = $state("");
	let signInPassword = $state("");

	let error = $state("");
	let success = $state("");

	// Clear messages after 4 seconds
	function fadeAlert() {
		setTimeout(() => {
			success = "";
			error = "";
		}, 2000);
	}

	function resetForm() {
		fName = "";
		lName = "";
		email = "";
		password = "";
		confirmPassword = "";
		signInEmail = "";
		signInPassword = "";
		error = "";
		success = "";
	}

	function toggleMode() {
		isSignUp = !isSignUp;
		resetForm();
	}

	async function handleSignUp() {
		// Validation
		if (
			!fName.trim() ||
			!lName.trim() ||
			!email.trim() ||
			!password.trim()
		) {
			error = "All fields are required";
			success = "";
			fadeAlert();
			return;
		}

		if (password.length < 6) {
			error = "Password must be at least 6 characters long";
			success = "";
			fadeAlert();
			return;
		}

		if (password !== confirmPassword) {
			error = "Passwords do not match";
			success = "";
			fadeAlert();
			return;
		}

		const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
		if (!emailRegex.test(email)) {
			error = "Please enter a valid email address";
			success = "";
			fadeAlert();
			return;
		}

		isLoading = true;

		try {
			if (signUp(fName, lName, password, email)) {
				success = "Account created successfully! Redirecting...";
				error = "";
				fadeAlert();

				// Delay redirect to show success message
				setTimeout(() => {
					window.location.hash = "#/";   // go to home
		            window.location.reload();
				}, 1000);
			} else {
				error =
					"An account with this email already exists. Please sign in.";
				success = "";
				fadeAlert();

				// Switch to sign in mode after a delay
				setTimeout(() => {
					isSignUp = false;
					signInEmail = email;
					resetForm();
				}, 1000);
			}
		} catch (err) {
			error = "An unexpected error occurred. Please try again.";
			success = "";
			fadeAlert();
		} finally {
			isLoading = false;
		}
	}

	async function handleSignIn() {
		if (!signInEmail.trim() || !signInPassword.trim()) {
			error = "Email and password are required";
			success = "";
			fadeAlert();
			return;
		}

		isLoading = true;

		try {
			if (login(signInEmail, signInPassword)) {
				success = "Welcome back! Redirecting...";
				error = "";
				fadeAlert();

				setTimeout(() => {
					window.location.hash = "#/";   // go to home
		            window.location.reload();
				}, 1000);
			} else {
				error = "Invalid email or password. Please try again.";
				success = "";
				fadeAlert();
			}
		} catch (err) {
			error = "An unexpected error occurred. Please try again.";
			success = "";
			fadeAlert();
		} finally {
			isLoading = false;
		}
	}
</script>

<main
	class="min-h-screen bg-gradient-to-br from-primary/10 via-base-200 to-secondary/10 flex items-center justify-center p-4"
>
	<div class="w-full max-w-150">
		<!-- Logo and Welcome Section -->
		<div class="text-center items-center justify-center flex flex-col">
			<div class="flex flex-col items-center mb-3">
				<h1
					class="text-4xl text-center text-warning/90 font-extrabold"
				>
					{!showlogin ? "Join Us" : "Welcome Back"}
				</h1>
			</div>
			<p class="text-base-content/70 text-lg mb-4">
				{!showlogin
					? "Create your account to start managing your finances"
					: "Sign in to access your wallet dashboard"}
			</p>
		</div>

		<!-- Main Card -->
		<div class="card bg-base-100 p-2 shadow-2xl border border-base-300">
			<div class="card-body space-y-6">
				<!-- Alert Messages -->
				{#if error}
					<div class="alert alert-error shadow-lg">
						<svg
							xmlns="http://www.w3.org/2000/svg"
							class="stroke-current shrink-0 h-6 w-6"
							fill="none"
							viewBox="0 0 24 24"
						>
							<path
								stroke-linecap="round"
								stroke-linejoin="round"
								stroke-width="2"
								d="M10 14l2-2m0 0l2-2m-2 2l-2-2m2 2l2 2m7-2a9 9 0 11-18 0 9 9 0 0118 0z"
							/>
						</svg>
						<span class="font-medium">{error}</span>
					</div>
				{/if}

				{#if success}
					<div class="alert alert-success shadow-lg">
						<svg
							xmlns="http://www.w3.org/2000/svg"
							class="stroke-current shrink-0 h-6 w-6"
							fill="none"
							viewBox="0 0 24 24"
						>
							<path
								stroke-linecap="round"
								stroke-linejoin="round"
								stroke-width="2"
								d="M9 12l2 2 4-4m6 2a9 9 0 11-18 0 9 9 0 0118 0z"
							/>
						</svg>
						<span class="font-medium">{success}</span>
					</div>
				{/if}

				{#if !showlogin}
					<!-- Sign Up Form -->
					<form onsubmit={handleSignUp} class="space-y-4">
						<!-- Name Fields -->
						<div class="grid grid-cols-2 gap-3">
							<div class="form-control">
								<label class="label mb-2" for="firstName">
									<span class="label mb-2-text font-medium"
										>First Name</span
									>
								</label>
								<input
									id="firstName"
									type="text"
									placeholder="Enter first name"
									bind:value={fName}
									disabled={isLoading}
									class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
									required
								/>
							</div>
							<div class="form-control">
								<label class="label mb-2" for="lastName">
									<span class="label mb-2-text font-medium"
										>Last Name</span
									>
								</label>
								<input
									id="lastName"
									type="text"
									placeholder="Enter last name"
									bind:value={lName}
									disabled={isLoading}
									class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
									required
								/>
							</div>
						</div>

						<!-- Email Field -->
						<div class="form-control">
							<label class="label mb-2" for="email">
								<span class="label mb-2-text font-medium"
									>Email Address</span
								>
							</label>
							<input
								id="email"
								type="email"
								placeholder="Enter your email"
								bind:value={email}
								disabled={isLoading}
								class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
								required
							/>
						</div>

						<!-- Password Fields -->
						<div class="form-control">
							<label class="label mb-2" for="password">
								<span class="label mb-2-text font-medium"
									>Password</span
								>
							</label>
							<input
								id="password"
								type="password"
								placeholder="Create a password"
								bind:value={password}
								disabled={isLoading}
								class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
								required
								minlength="6"
							/>
							<label class="label">
								<span class="label mt-1 text-base-content/60"
									>Must be at least 6 characters</span
								>
							</label>
						</div>

						<div class="form-control">
							<label class="label mb-2" for="confirmPassword">
								<span class="label mb-2-text font-medium"
									>Confirm Password</span
								>
							</label>
							<input
								id="confirmPassword"
								type="password"
								placeholder="Confirm your password"
								bind:value={confirmPassword}
								disabled={isLoading}
								class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
								required
							/>
						</div>

						<!-- Sign Up Button -->
						<button
							type="submit"
							class="btn btn-primary w-full btn-lg"
							disabled={isLoading}
						>
							{#if isLoading}
								<span class="loading loading-spinner loading-md"
								></span>
								Creating Account...
							{:else}
								<svg
									xmlns="http://www.w3.org/2000/svg"
									class="h-5 w-5"
									fill="none"
									viewBox="0 0 24 24"
									stroke="currentColor"
								>
									<path
										stroke-linecap="round"
										stroke-linejoin="round"
										stroke-width="2"
										d="M18 9v3m0 0v3m0-3h3m-3 0h-3m-2-5a4 4 0 11-8 0 4 4 0 018 0zM3 20a6 6 0 0112 0v1H3v-1z"
									/>
								</svg>
								Create Account
							{/if}
						</button>
					</form>
				{:else}
					<!-- Sign In Form -->
					<form onsubmit={handleSignIn} class="space-y-4">
						<!-- Email Field -->
						<div class="form-control">
							<label class="label mb-2" for="signInEmail">
								<span class="label mb-2-text font-medium"
									>Email Address</span
								>
							</label>
							<input
								id="signInEmail"
								type="email"
								placeholder="Enter your email"
								bind:value={signInEmail}
								disabled={isLoading}
								class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
								required
							/>
						</div>

						<!-- Password Field -->
						<div class="form-control">
							<label class="label mb-2" for="signInPassword">
								<span class="label mb-2-text font-medium"
									>Password</span
								>
							</label>
							<input
								id="signInPassword"
								type="password"
								placeholder="Enter your password"
								bind:value={signInPassword}
								disabled={isLoading}
								class="input input-bordered w-full focus:input-warning outline-none rounded-xl"
								required
							/>
						</div>

						<!-- Sign In Button -->
						<button
							type="submit"
							class="btn btn-primary w-full mt-2 btn-lg"
							disabled={isLoading}
						>
							{#if isLoading}
								<span class="loading loading-spinner loading-md"
								></span>
								Signing In...
							{:else}
								<svg
									xmlns="http://www.w3.org/2000/svg"
									class="h-5 w-5"
									fill="none"
									viewBox="0 0 24 24"
									stroke="currentColor"
								>
									<path
										stroke-linecap="round"
										stroke-linejoin="round"
										stroke-width="2"
										d="M11 16l-4-4m0 0l4-4m-4 4h14m-5 4v1a3 3 0 01-3 3H6a3 3 0 01-3-3V7a3 3 0 013-3h7a3 3 0 013 3v1"
									/>
								</svg>
								Sign In
							{/if}
						</button>
					</form>
				{/if}
			</div>
		</div>

		<!-- Footer -->
		<div class="text-center mt-8 text-base-content/60">
			<p class="text-sm">
				By continuing, you agree to our
				<label
					for="license_modal"
					class="link link-primary cursor-pointer"
				>
					Terms of Service
				</label>
				and acknowledge our Privacy Policy.
			</p>
		</div>

		<!-- DaisyUI Modal -->
		<input type="checkbox" id="license_modal" class="modal-toggle" />
		<div class="modal">
			<div class="modal-box max-w-3xl p-6 pb-0">
				<header
					class="text-center border-b border-base-100 pt-6 pb-3 mb-8"
				>
					<h1 class="text-4xl font-extrabold text-warning mb-2">
						{licenseName}
					</h1>
				</header>

				<div class="flex-1 pb-8">
					<p class="font-semibold text-base-content text-center mb-3">
						© {copyrightYear}
						{companyName}. All rights reserved.
					</p>
					<div
						class="bg-base-100 p-4 rounded-xl shadow-md border-l-4 border-warning whitespace-pre-wrap max-h-[60vh] overflow-auto"
					>
						<p
							class="text-base-content sm:text-lg font-medium text-justify leading-relaxed"
						>
							{licenseText}
						</p>
					</div>
					<div class="modal-action">
						<label for="license_modal" class="btn">Close</label>
					</div>
				</div>
			</div>
		</div>
	</div>
</main>
