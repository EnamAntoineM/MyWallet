<script lang="ts">
  // AllTransactions.svelte - Complete transactions management page with dark theme
  
  interface Transaction {
    id: number;
    wallet_name: string;
    type: 'INCOME' | 'EXPENSE' | 'TRANSFER';
    recorded_at: string;
    updated_at: string;
    category: string;
    description: string;
    amount: number;
    is_archived: boolean;
    related_wallet_name: string;
  }

  interface Wallet {
    id: number;
    name: string;
    currency: string;
    source: string;
    initial_amount: number;
    balance: number;
    color: string;
    created_at: string;
    updated_at: string;
    is_active: boolean;
  }

  // State management
  let transactions: Transaction[] = [];
  let wallets: Wallet[] = [];
  let filteredTransactions: Transaction[] = [];
  let isLoading = false;
  let message = '';
  let messageType: 'success' | 'error' | 'info' = 'info';

  // Filter states
  let searchTerm = '';
  let selectedWallet = '';
  let selectedType = '';
  let selectedCategory = '';
  let dateFrom = '';
  let dateTo = '';
  let showArchived = false;

  // Pagination
  let currentPage = 1;
  let itemsPerPage = 20;
  let totalPages = 1;

  // Modal states
  let showDeleteModal = false;
  let transactionToDelete: Transaction | null = null;

  // Load data on component mount
  $: {
    loadAllData();
  }

  // Apply filters when any filter changes
  $: {
    applyFilters();
  }

  // Calculate pagination
  $: {
    totalPages = Math.ceil(filteredTransactions.length / itemsPerPage);
  }

  // Get transactions for current page
  $: paginatedTransactions = filteredTransactions.slice(
    (currentPage - 1) * itemsPerPage,
    currentPage * itemsPerPage
  );

  async function loadAllData() {
    isLoading = true;
    await Promise.all([loadTransactions(), loadWallets()]);
    isLoading = false;
  }

  async function loadTransactions() {
    try {
      // Use empty criteria to get all transactions
      const result = await (window as any).get_records('{}');
      const parsed = JSON.parse(result);
      
      if (Array.isArray(parsed)) {
        transactions = parsed.map(t => ({
          ...t,
          recorded_at: formatDate(t.recorded_at),
          updated_at: formatDate(t.updated_at)
        }));
        showMessage(`Loaded ${transactions.length} transactions`, 'success');
      } else {
        transactions = [];
        showMessage('No transactions found', 'info');
      }
    } catch (error) {
      console.error('Error loading transactions:', error);
      showMessage('Failed to load transactions', 'error');
      transactions = [];
    }
  }

  async function loadWallets() {
    try {
      const result = await (window as any).get_wallets('{}');
      const parsed = JSON.parse(result);
      
      if (Array.isArray(parsed)) {
        wallets = parsed.filter(w => w.is_active);
      } else {
        wallets = [];
      }
    } catch (error) {
      console.error('Error loading wallets:', error);
      wallets = [];
    }
  }

  function applyFilters() {
    filteredTransactions = transactions.filter(transaction => {
      // Search term filter
      if (searchTerm) {
        const searchLower = searchTerm.toLowerCase();
        const matchesSearch = 
          transaction.description.toLowerCase().includes(searchLower) ||
          transaction.category.toLowerCase().includes(searchLower) ||
          transaction.wallet_name.toLowerCase().includes(searchLower) ||
          transaction.related_wallet_name.toLowerCase().includes(searchLower);
        
        if (!matchesSearch) return false;
      }

      // Wallet filter
      if (selectedWallet && transaction.wallet_name !== selectedWallet) {
        return false;
      }

      // Type filter
      if (selectedType && transaction.type !== selectedType) {
        return false;
      }

      // Category filter
      if (selectedCategory && transaction.category !== selectedCategory) {
        return false;
      }

      // Date range filter
      if (dateFrom) {
        const transactionDate = new Date(transaction.recorded_at);
        const fromDate = new Date(dateFrom);
        if (transactionDate < fromDate) return false;
      }

      if (dateTo) {
        const transactionDate = new Date(transaction.recorded_at);
        const toDate = new Date(dateTo);
        if (transactionDate > toDate) return false;
      }

      // Archived filter
      if (!showArchived && transaction.is_archived) {
        return false;
      }

      return true;
    });

    // Reset pagination when filters change
    currentPage = 1;
  }

  function clearFilters() {
    searchTerm = '';
    selectedWallet = '';
    selectedType = '';
    selectedCategory = '';
    dateFrom = '';
    dateTo = '';
    showArchived = false;
  }

  async function deleteTransaction(transaction: Transaction) {
    try {
      const result = await (window as any).delete_record(JSON.stringify({
        id: transaction.id
      }));
      
      if (result.includes('successfully')) {
        showMessage('Transaction deleted successfully', 'success');
        await loadTransactions();
      } else {
        showMessage(result, 'error');
      }
    } catch (error) {
      showMessage(`Error deleting transaction: ${error}`, 'error');
    }
    
    closeDeleteModal();
  }

  function openDeleteModal(transaction: Transaction) {
    transactionToDelete = transaction;
    showDeleteModal = true;
  }

  function closeDeleteModal() {
    transactionToDelete = null;
    showDeleteModal = false;
  }

  function showMessage(msg: string, type: 'success' | 'error' | 'info') {
    message = msg;
    messageType = type;
    
    setTimeout(() => {
      message = '';
    }, 5000);
  }

  function formatDate(dateStr: string): string {
    try {
      return new Date(dateStr).toLocaleDateString() + ' ' + 
             new Date(dateStr).toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'});
    } catch {
      return dateStr;
    }
  }

  function formatAmount(amount: number): string {
    return amount.toLocaleString();
  }

  function getTypeColor(type: string): string {
    switch (type) {
      case 'INCOME': return 'text-emerald-400 bg-emerald-500/10 border border-emerald-500/20';
      case 'EXPENSE': return 'text-rose-400 bg-rose-500/10 border border-rose-500/20';
      case 'TRANSFER': return 'text-blue-400 bg-blue-500/10 border border-blue-500/20';
      default: return 'text-gray-400 bg-gray-500/10 border border-gray-500/20';
    }
  }

  function getUniqueCategories(): string[] {
    const categories = [...new Set(transactions.map(t => t.category).filter(Boolean))];
    return categories.sort();
  }

  function changePage(page: number) {
    if (page >= 1 && page <= totalPages) {
      currentPage = page;
    }
  }
</script>

<div class="all-transactions-container">
  <div class="header">
    <h1 class="title">All Transactions</h1>
    <div class="header-actions">
      <button on:click={loadAllData} class="btn-refresh" disabled={isLoading}>
        {isLoading ? 'Loading...' : 'Refresh'}
      </button>
    </div>
  </div>

  {#if message}
    <div class="message {messageType}">
      {message}
    </div>
  {/if}

  <!-- Filters Section -->
  <div class="filters-section">
    <div class="filters-grid">
      <div class="filter-group">
        <label for="search">Search:</label>
        <input 
          id="search"
          type="text" 
          bind:value={searchTerm} 
          placeholder="Search description, category, wallet..."
        />
      </div>

      <div class="filter-group">
        <label for="wallet-filter">Wallet:</label>
        <select id="wallet-filter" bind:value={selectedWallet}>
          <option value="">All Wallets</option>
          {#each wallets as wallet}
            <option value={wallet.name}>{wallet.name}</option>
          {/each}
        </select>
      </div>

      <div class="filter-group">
        <label for="type-filter">Type:</label>
        <select id="type-filter" bind:value={selectedType}>
          <option value="">All Types</option>
          <option value="INCOME">Income</option>
          <option value="EXPENSE">Expense</option>
          <option value="TRANSFER">Transfer</option>
        </select>
      </div>

      <div class="filter-group">
        <label for="category-filter">Category:</label>
        <select id="category-filter" bind:value={selectedCategory}>
          <option value="">All Categories</option>
          {#each getUniqueCategories() as category}
            <option value={category}>{category}</option>
          {/each}
        </select>
      </div>

      <div class="filter-group">
        <label for="date-from">From Date:</label>
        <input id="date-from" type="date" bind:value={dateFrom} />
      </div>

      <div class="filter-group">
        <label for="date-to">To Date:</label>
        <input id="date-to" type="date" bind:value={dateTo} />
      </div>

      <div class="filter-group checkbox-group">
        <label>
          <input 
            type="checkbox" 
            bind:checked={showArchived}
          />
          Show Archived
        </label>
      </div>

      <div class="filter-actions">
        <button on:click={clearFilters} class="btn-clear">
          Clear Filters
        </button>
      </div>
    </div>
  </div>

  <!-- Summary -->
  <div class="summary">
    <span class="summary-item">
      Total: {filteredTransactions.length} transactions
    </span>
    <span class="summary-item">
      Page {currentPage} of {totalPages}
    </span>
  </div>

  <!-- Transactions Table -->
  <div class="table-container">
    {#if isLoading}
      <div class="loading">Loading transactions...</div>
    {:else if paginatedTransactions.length === 0}
      <div class="empty-state">
        <p>No transactions found</p>
        {#if searchTerm || selectedWallet || selectedType || selectedCategory || dateFrom || dateTo}
          <button on:click={clearFilters} class="btn-clear">Clear filters to see all transactions</button>
        {/if}
      </div>
    {:else}
      <table class="transactions-table">
        <thead>
          <tr>
            <th>Date</th>
            <th>Type</th>
            <th>Wallet</th>
            <th>Category</th>
            <th>Description</th>
            <th>Amount</th>
            <th>Related Wallet</th>
            <th>Actions</th>
          </tr>
        </thead>
        <tbody>
          {#each paginatedTransactions as transaction}
            <tr class:archived={transaction.is_archived}>
              <td>{formatDate(transaction.recorded_at)}</td>
              <td>
                <span class="type-badge {getTypeColor(transaction.type)}">
                  {transaction.type}
                </span>
              </td>
              <td>{transaction.wallet_name}</td>
              <td>{transaction.category}</td>
              <td class="description">{transaction.description}</td>
              <td class="amount">
                {formatAmount(transaction.amount)}
              </td>
              <td>{transaction.related_wallet_name || '-'}</td>
              <td class="actions">
                <button 
                  class="btn-edit"
                  on:click={() => {
                    // Navigate to edit page - you'll need to implement routing
                    if (transaction.type === 'TRANSFER') {
                      window.location.href = `/transfer/update/${transaction.id}`;
                    } else {
                      window.location.href = `/transaction/update/${transaction.id}`;
                    }
                  }}
                >
                  Edit
                </button>
                <button 
                  class="btn-delete"
                  on:click={() => openDeleteModal(transaction)}
                >
                  Delete
                </button>
              </td>
            </tr>
          {/each}
        </tbody>
      </table>
    {/if}
  </div>

  <!-- Pagination -->
  {#if totalPages > 1}
    <div class="pagination">
      <button 
        on:click={() => changePage(currentPage - 1)}
        disabled={currentPage === 1}
        class="btn-page"
      >
        Previous
      </button>
      
      {#each Array(totalPages) as _, i}
        <button 
          on:click={() => changePage(i + 1)}
          class="btn-page {currentPage === i + 1 ? 'active' : ''}"
        >
          {i + 1}
        </button>
      {/each}
      
      <button 
        on:click={() => changePage(currentPage + 1)}
        disabled={currentPage === totalPages}
        class="btn-page"
      >
        Next
      </button>
    </div>
  {/if}
</div>

<!-- Delete Confirmation Modal -->
{#if showDeleteModal && transactionToDelete}
  <!-- svelte-ignore a11y_click_events_have_key_events -->
  <!-- svelte-ignore a11y_no_static_element_interactions -->
  <div class="modal-backdrop" on:click={closeDeleteModal}>
    <div class="modal" on:click|stopPropagation>
      <h3>Confirm Delete</h3>
      <p>Are you sure you want to delete this transaction?</p>
      <div class="transaction-details">
        <strong>Type:</strong> {transactionToDelete.type}<br>
        <strong>Amount:</strong> {formatAmount(transactionToDelete.amount)}<br>
        <strong>Description:</strong> {transactionToDelete.description}<br>
        <strong>Date:</strong> {formatDate(transactionToDelete.recorded_at)}
      </div>
      <div class="modal-actions">
        <button on:click={closeDeleteModal} class="btn-cancel">Cancel</button>
        <button on:click={() => deleteTransaction(transactionToDelete!)} class="btn-confirm-delete">
          Delete
        </button>
      </div>
    </div>
  </div>
{/if}

<style>
  :global(body) {
    background: #1a1f2e;
    color: #e2e8f0;
    font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
    margin: 0;
    padding: 0;
  }

  .all-transactions-container {
    background: #1a1f2e;
    min-height: 100vh;
    padding: 1.5rem;
    max-width: 1400px;
    margin: 0 auto;
    color: #e2e8f0;
  }

  .header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 2rem;
  }

  .title {
    font-size: 2rem;
    font-weight: 600;
    color: #f1f5f9;
    margin: 0;
  }

  .header-actions {
    display: flex;
    gap: 1rem;
  }

  .btn-refresh {
    background: linear-gradient(135deg, #3b82f6, #1d4ed8);
    color: white;
    border: none;
    padding: 0.625rem 1.25rem;
    border-radius: 8px;
    cursor: pointer;
    font-weight: 500;
    transition: all 0.2s;
    box-shadow: 0 2px 4px rgba(59, 130, 246, 0.3);
  }

  .btn-refresh:hover:not(:disabled) {
    background: linear-gradient(135deg, #2563eb, #1e40af);
    transform: translateY(-1px);
    box-shadow: 0 4px 8px rgba(59, 130, 246, 0.4);
  }

  .btn-refresh:disabled {
    background: #475569;
    cursor: not-allowed;
    transform: none;
    box-shadow: none;
  }

  .message {
    padding: 1rem 1.25rem;
    border-radius: 8px;
    margin-bottom: 1.5rem;
    font-weight: 500;
    border: 1px solid transparent;
  }

  .message.success {
    background: rgba(16, 185, 129, 0.1);
    color: #34d399;
    border-color: rgba(16, 185, 129, 0.2);
  }

  .message.error {
    background: rgba(239, 68, 68, 0.1);
    color: #f87171;
    border-color: rgba(239, 68, 68, 0.2);
  }

  .message.info {
    background: rgba(59, 130, 246, 0.1);
    color: #60a5fa;
    border-color: rgba(59, 130, 246, 0.2);
  }

  .filters-section {
    background: #2d3748;
    border: 1px solid #4a5568;
    border-radius: 12px;
    padding: 1.5rem;
    margin-bottom: 1.5rem;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  }

  .filters-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
    gap: 1rem;
    align-items: end;
  }

  .filter-group {
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
  }

  .checkbox-group {
    flex-direction: row;
    align-items: center;
  }

  .checkbox-group label {
    display: flex;
    align-items: center;
    gap: 0.5rem;
    cursor: pointer;
  }

  .filter-group label {
    font-weight: 500;
    color: #cbd5e0;
    font-size: 0.875rem;
  }

  .filter-group input,
  .filter-group select {
    background: #374151;
    color: #e2e8f0;
    padding: 0.625rem;
    border: 1px solid #4b5563;
    border-radius: 8px;
    font-size: 0.875rem;
    transition: all 0.2s;
  }

  .filter-group input:focus,
  .filter-group select:focus {
    outline: none;
    border-color: #3b82f6;
    box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.1);
    background: #3f444e;
  }

  .filter-group input::placeholder {
    color: #9ca3af;
  }

  .filter-group input[type="checkbox"] {
    width: 1rem;
    height: 1rem;
    accent-color: #3b82f6;
  }

  .btn-clear {
    background: #374151;
    color: #e2e8f0;
    border: 1px solid #4b5563;
    padding: 0.625rem 1.25rem;
    border-radius: 8px;
    cursor: pointer;
    font-size: 0.875rem;
    transition: all 0.2s;
  }

  .btn-clear:hover {
    background: #4b5563;
    border-color: #6b7280;
  }

  .summary {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 1rem;
    font-size: 0.875rem;
    color: #94a3b8;
  }

  .table-container {
    background: #2d3748;
    border: 1px solid #4a5568;
    border-radius: 12px;
    overflow: hidden;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  }

  .loading, .empty-state {
    text-align: center;
    padding: 3rem;
    color: #94a3b8;
  }

  .transactions-table {
    width: 100%;
    border-collapse: collapse;
  }

  .transactions-table th {
    background: #374151;
    padding: 1rem 0.75rem;
    text-align: left;
    font-weight: 600;
    color: #f1f5f9;
    border-bottom: 1px solid #4b5563;
    font-size: 0.875rem;
  }

  .transactions-table td {
    padding: 1rem 0.75rem;
    border-bottom: 1px solid #3a4553;
    font-size: 0.875rem;
    color: #e2e8f0;
  }

  .transactions-table tbody tr:hover {
    background: rgba(59, 130, 246, 0.05);
  }

  .transactions-table tbody tr.archived {
    opacity: 0.5;
    background: rgba(0, 0, 0, 0.1);
  }

  .type-badge {
    padding: 0.375rem 0.75rem;
    border-radius: 6px;
    font-size: 0.75rem;
    font-weight: 600;
    text-transform: uppercase;
    letter-spacing: 0.025em;
  }

  .description {
    max-width: 200px;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }

  .amount {
    text-align: right;
    font-weight: 600;
    font-family: 'Courier New', monospace;
  }

  .actions {
    display: flex;
    gap: 0.5rem;
  }

  .btn-edit, .btn-delete {
    padding: 0.375rem 0.75rem;
    border-radius: 6px;
    font-size: 0.75rem;
    font-weight: 500;
    cursor: pointer;
    transition: all 0.2s;
    border: 1px solid transparent;
  }

  .btn-edit {
    background: rgba(59, 130, 246, 0.1);
    color: #60a5fa;
    border-color: rgba(59, 130, 246, 0.2);
  }

  .btn-edit:hover {
    background: rgba(59, 130, 246, 0.2);
    transform: translateY(-1px);
  }

  .btn-delete {
    background: rgba(239, 68, 68, 0.1);
    color: #f87171;
    border-color: rgba(239, 68, 68, 0.2);
  }

  .btn-delete:hover {
    background: rgba(239, 68, 68, 0.2);
    transform: translateY(-1px);
  }

  .pagination {
    display: flex;
    justify-content: center;
    gap: 0.5rem;
    margin-top: 1.5rem;
  }

  .btn-page {
    padding: 0.625rem 1rem;
    border: 1px solid #4b5563;
    background: #374151;
    color: #e2e8f0;
    border-radius: 8px;
    cursor: pointer;
    font-size: 0.875rem;
    transition: all 0.2s;
  }

  .btn-page:hover:not(:disabled) {
    background: #4b5563;
    transform: translateY(-1px);
  }

  .btn-page:disabled {
    opacity: 0.5;
    cursor: not-allowed;
    transform: none;
  }

  .btn-page.active {
    background: linear-gradient(135deg, #3b82f6, #1d4ed8);
    color: white;
    border-color: #3b82f6;
  }

  .modal-backdrop {
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: rgba(0, 0, 0, 0.7);
    display: flex;
    align-items: center;
    justify-content: center;
    z-index: 1000;
    backdrop-filter: blur(4px);
  }

  .modal {
    background: #2d3748;
    border: 1px solid #4a5568;
    padding: 2rem;
    border-radius: 12px;
    max-width: 500px;
    width: 90%;
    box-shadow: 0 20px 25px rgba(0, 0, 0, 0.3);
  }

  .modal h3 {
    margin: 0 0 1rem 0;
    color: #f1f5f9;
    font-size: 1.25rem;
  }

  .modal p {
    color: #cbd5e0;
    margin-bottom: 1rem;
  }

  .transaction-details {
    background: #374151;
    border: 1px solid #4b5563;
    padding: 1rem;
    border-radius: 8px;
    margin: 1rem 0;
    font-size: 0.875rem;
    line-height: 1.6;
    color: #e2e8f0;
  }

  .modal-actions {
    display: flex;
    gap: 1rem;
    margin-top: 1.5rem;
  }

  .btn-cancel, .btn-confirm-delete {
    flex: 1;
    padding: 0.75rem;
    border-radius: 8px;
    cursor: pointer;
    font-weight: 500;
    transition: all 0.2s;
    border: 1px solid transparent;
  }

  .btn-cancel {
    background: #374151;
    color: #e2e8f0;
    border-color: #4b5563;
  }

  .btn-cancel:hover {
    background: #4b5563;
  }

  .btn-confirm-delete {
    background: linear-gradient(135deg, #dc2626, #b91c1c);
    color: white;
  }

  .btn-confirm-delete:hover {
    background: linear-gradient(135deg, #b91c1c, #991b1b);
    transform: translateY(-1px);
  }

  @media (max-width: 768px) {
    .all-transactions-container {
      padding: 1rem;
    }

    .header {
      flex-direction: column;
      gap: 1rem;
    }

    .filters-grid {
      grid-template-columns: 1fr;
    }

    .table-container {
      overflow-x: auto;
    }

    .transactions-table {
      min-width: 800px;
    }

    .summary {
      flex-direction: column;
      gap: 0.5rem;
      align-items: flex-start;
    }
  }
</style>