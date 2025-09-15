<script lang="ts">
	import type { Wallet } from "$lib/index";
	
	
	export let wallet: Wallet;
	export let isOpen: boolean = false;
	export let onSave: (wallet: Wallet) => void;
	export let onClose: () => void;

	// Local form data
	let formData: Wallet = $state({
		id: 0,
		name: "",
		currency: "USD",
		source: "",
		initial_amount: 0,
		balance: 0,
		color: "#3b82f6",
		created_at: "",
		updated_at: "",
		is_active: true,
	});

	
	$effect(() => {
		if (wallet) {
			formData = { ...wallet };
		}
	});

	function handleSubmit() {
		onSave(formData);
	}

	function handleCancel() {
		onClose();
	}
</script>

{#if isOpen}
<div class="modal modal-open" on:click|self={onClose}>
	<div class="modal-box" on:click|stopPropagation>
		<h3 class="font-bold text-lg mb-6">{formData.id ? 'Edit' : 'Add'} Wallet</h3>
		<form on:submit|preventDefault={handleSubmit} class="space-y-4">
			<!-- Wallet Name -->
			<div class="form-control">
				<label class="label">
					<span class="label-text font-semibold">Wallet Name</span>
				</label>
				<input 
					type="text" 
					class="input input-bordered w-full" 
					bind:value={formData.name}
					required
					placeholder="e.g., Personal Wallet, Business Account"
				/>
			</div>

			<!-- Currency -->
			<div class="form-control">
				<label class="label">
					<span class="label-text font-semibold">Currency</span>
				</label>
				<select class="select select-bordered w-full" bind:value={formData.currency}>
					<option value="USD">USD ($)</option>
					<option value="EUR">EUR (€)</option>
					<option value="GBP">GBP (£)</option>
					<option value="JPY">JPY (¥)</option>
					<option value="CAD">CAD (C$)</option>
					<option value="AUD">AUD (A$)</option>
				</select>
			</div>

			<!-- Source -->
			<div class="form-control">
				<label class="label">
					<span class="label-text font-semibold">Source</span>
				</label>
				<input 
					type="text" 
					class="input input-bordered w-full" 
					bind:value={formData.source}
					placeholder="e.g., Bank of America, Cash, PayPal"
				/>
			</div>

			<!-- Initial Amount -->
			<div class="form-control">
				<label class="label">
					<span class="label-text font-semibold">Initial Amount</span>
				</label>
				<input 
					type="number" 
					class="input input-bordered w-full" 
					bind:value={formData.initial_amount}
					step="0.01"
					min="0"
					placeholder="0.00"
				/>
			</div>

			<!-- Wallet Color -->
			<div class="form-control">
				<label class="label">
					<span class="label-text font-semibold">Wallet Color</span>
				</label>
				<div class="flex items-center gap-4">
					<input 
						type="color" 
						class="w-12 h-12 rounded cursor-pointer border border-gray-300" 
						bind:value={formData.color}
					/>
					<span class="text-sm text-gray-600">Choose a color for this wallet</span>
				</div>
			</div>


			<div class="modal-action mt-8">
				<button type="button" class="btn btn-ghost" on:click={handleCancel}>
					Cancel
				</button>
				<button type="submit" class="btn btn-primary">
					{formData.id ? 'Update' : 'Create'} Wallet
				</button>
			</div>
		</form>
	</div>
</div>
{/if}

<style>
	.modal {
		position: fixed;
		top: 0;
		left: 0;
		width: 100%;
		height: 100%;
		background: rgba(0, 0, 0, 0.5);
		display: flex;
		align-items: center;
		justify-content: center;
		z-index: 1000;
	}

	.modal-box {
		background: white;
		padding: 2rem;
		border-radius: 12px;
		max-width: 500px;
		width: 90%;
		max-height: 90vh;
		overflow-y: auto;
		box-shadow: 0 20px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04);
	}

	.form-control {
		margin-bottom: 1.5rem;
	}

	.label {
		display: block;
		margin-bottom: 0.5rem;
	}

	.label-text {
		font-weight: 600;
		color: #374151;
		font-size: 0.95rem;
	}

	.input, .select {
		width: 100%;
		padding: 0.75rem;
		border: 1px solid #d1d5db;
		border-radius: 8px;
		background: white;
		font-size: 0.95rem;
		transition: border-color 0.2s ease, box-shadow 0.2s ease;
	}

	.input:focus, .select:focus {
		outline: none;
		border-color: #3b82f6;
		box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.1);
	}

	.modal-action {
		display: flex;
		justify-content: flex-end;
		gap: 1rem;
		margin-top: 2rem;
		padding-top: 1.5rem;
		border-top: 1px solid #e5e7eb;
	}

	.btn {
		padding: 0.75rem 1.5rem;
		border-radius: 8px;
		font-weight: 600;
		font-size: 0.95rem;
		transition: all 0.2s ease;
		cursor: pointer;
	}

	.btn-ghost {
		background: transparent;
		color: #6b7280;
		border: 1px solid #d1d5db;
	}

	.btn-ghost:hover {
		background: #f9fafb;
		color: #374151;
	}

	.btn-primary {
		background: #3b82f6;
		color: white;
		border: 1px solid #3b82f6;
	}

	.btn-primary:hover {
		background: #2563eb;
		border-color: #2563eb;
	}

	
	.modal-box::-webkit-scrollbar {
		width: 6px;
	}

	.modal-box::-webkit-scrollbar-track {
		background: #f1f1f1;
		border-radius: 3px;
	}

	.modal-box::-webkit-scrollbar-thumb {
		background: #c1c1c1;
		border-radius: 3px;
	}

	.modal-box::-webkit-scrollbar-thumb:hover {
		background: #a8a8a8;
	}
</style>