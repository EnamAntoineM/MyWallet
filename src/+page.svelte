
<script lang="ts">
 import type { Wallet, Transaction } from "$lib/index";
</script>

<svelte:head>
  <title>All Transactions - My Wallet</title>
  <meta name="description" content="View and manage all your financial transactions" />
</svelte:head>

<AllTransactions />

<style>
  :global(body) {
    background-color: #1a1f2e;
    color: #e2e8f0;
    margin: 0;
    padding: 0;
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;
  }

  :global(*) {
    box-sizing: border-box;
  }

  :global(html) {
    background-color: #1a1f2e;
  }

  /* Ensure all scrollbars match the dark theme */
  :global(::-webkit-scrollbar) {
    width: 12px;
  }

  :global(::-webkit-scrollbar-track) {
    background: #2d3748;
  }

  :global(::-webkit-scrollbar-thumb) {
    background: #4a5568;
    border-radius: 6px;
  }

  :global(::-webkit-scrollbar-thumb:hover) {
    background: #718096;
  }

  /* Dark theme for form elements */
  :global(input),
  :global(select),
  :global(textarea) {
    background-color: #374151 !important;
    color: #e2e8f0 !important;
    border: 1px solid #4b5563 !important;
  }

  :global(input:focus),
  :global(select:focus),
  :global(textarea:focus) {
    border-color: #3b82f6 !important;
    box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.1) !important;
  }

  :global(input::placeholder) {
    color: #9ca3af !important;
  }

  /* Dark theme for buttons */
  :global(button) {
    font-family: inherit;
  }

  /* Remove any unwanted white backgrounds */
  :global(.bg-white) {
    background-color: #2d3748 !important;
  }

  :global(.text-gray-900) {
    color: #f1f5f9 !important;
  }

  :global(.text-gray-600) {
    color: #cbd5e0 !important;
  }

  :global(.border-gray-200) {
    border-color: #4a5568 !important;
  }

  :global(.border-gray-300) {
    border-color: #4b5563 !important;
  }
</style>

