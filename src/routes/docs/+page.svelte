<script lang="ts">
  import type { Wallet, Transaction } from "$lib/index";

  // Demo state (ids internal only; never shown/edited)
  let a_wallet: Wallet = $state({
    id: 0,
    name: "Frontend wallet",
    currency: "USD",
    source: "MOMO",
    initial_amount: 1000,
    balance: 800,
    color: "#7c3aed",
    created_at: "",
    updated_at: "",
    is_active: true
  });

  let a_record: Transaction = $state({
    id: 0,
    wallet_name: "Frontend wallet",
    type: "income",
    category: "transportation",
    description: "just to try",
    amount: 200,
    related_wallet_name: "",
    created_at: "",
    updated_at: "",
    is_archived: false
  });

  // Copy helper for code cards
  function copyCode(btn: HTMLButtonElement) {
    const card = btn.closest(".codecard");
    const code = card?.querySelector("code");
    if (!code) return;
    navigator.clipboard.writeText(code.textContent || "");
    btn.innerText = "Copied!";
    setTimeout(() => (btn.innerText = "Copy"), 1200);
  }
  function handleCopyClick(event: MouseEvent) {
    const btn = event.currentTarget as HTMLButtonElement;
    if (btn) copyCode(btn);
  }

  // Mobile TOC modal
  let showTOC = $state(false);
</script>

<!-- PAGE ROOT: lives under your fixed navbar, readable on white -->
<div
  data-theme="light"
  class="docs-root relative z-0 max-w-7xl mx-auto px-4 md:px-6 pb-24 pt-[40px] text-base-content bg-base-100"
>
  <!-- HEADER / HERO -->
  <header class="mb-10 md:mb-12 docs-hero fx-fade">
    <h1 class="docs-title">Wallet Tracker Documentation</h1>
    <p class="docs-subtitle">
      How to use the <strong>Svelte wrappers</strong> in your pages (with
      <code>onclick=&#123;...&#125;</code>), plus deep “why/what/how”
      explanations of the <code>$lib/index.ts</code> helpers and the Saucer
      → C++ contract. IDs are never shown or edited, timestamps may be displayed
      but are backend-authored.
    </p>
    <div class="flex gap-2 mt-4">
      <span class="badge badge-primary">Svelte&nbsp;5</span>
      <span class="badge badge-secondary">Tailwind</span>
      <span class="badge badge-accent">daisyUI</span>
      <span class="badge badge-outline">No IDs in UI</span>
    </div>
  </header>

  <!-- LAYOUT: MAIN + RIGHT TOC -->
  <div class="grid grid-cols-1 xl:grid-cols-[minmax(0,1fr)_300px] gap-8">
    <!-- ======================= MAIN CONTENT ======================= -->
    <main class="space-y-12">
      <!-- Guardrails -->
      <section id="guardrails" class="doc-section">
        <h2 class="doc-h2">Guardrails: IDs, timestamps &amp; UI rules</h2>
        <div class="card bg-base-200 shadow">
          <div class="card-body doc-prose">
            <ul>
              <li>
                <strong>Never display or edit IDs</strong> on the frontend. They are backend/DB concerns
                (monotonic/UUID; never reused). Use selection of an item (from a list) instead of asking for an ID.
              </li>
              <li>
                <strong>Timestamps</strong> (<code>created_at</code>, <code>updated_at</code>) are backend-authored.
                You may display them but <strong>never edit</strong> them.
              </li>
              <li>
                For list orientation, use an <em>arbitrary 1-based index</em> like <code>#1</code>, <code>#2</code> —
                <strong>not</strong> the real ID.
              </li>
              <li>
                All wrapper examples respect those rules. The object passed to wrappers carries the ID internally; you never show or edit it.
              </li>
            </ul>
          </div>
        </div>
      </section>

      <!-- ================= FIELD REFERENCE (STRUCTS) ================= -->
      <section id="field-reference" class="doc-section">
        <h2 class="doc-h2">Field Reference &amp; Rationale</h2>
        <p class="doc-lead">Each field’s purpose, ownership (frontend vs backend), constraints, and UI guidance. Emphasis on how values serialize to JSON and deserialize in C++.</p>

        <!-- Wallet -->
        <article class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">Wallet</h3>
            <div class="doc-prose">
              <div class="overflow-x-auto">
                <table class="table table-zebra">
                  <thead>
                    <tr>
                      <th>Field</th>
                      <th>What it means</th>
                      <th>Who sets it</th>
                      <th>Constraints &amp; notes</th>
                      <th>UI guidance</th>
                    </tr>
                  </thead>
                  <tbody>
                    <tr>
                      <td><code>id</code></td>
                      <td>Stable internal identity for the row in the DB.</td>
                      <td><strong>Backend</strong></td>
                      <td>Opaque (may be UUID, monotonic, sharded). Never reused.</td>
                      <td><strong>Never display or edit.</strong> Travels in objects you pass to wrappers only.</td>
                    </tr>
                    <tr>
                      <td><code>name</code></td>
                      <td>Human label like “Savings” or “Momo Wallet”.</td>
                      <td>Frontend input → <strong>validated/normalized by backend</strong></td>
                      <td>Non-empty; may require uniqueness per user; trimmed; case rules as policy.</td>
                      <td>Editable text input. Show as primary label in lists.</td>
                    </tr>
                    <tr>
                      <td><code>currency</code></td>
                      <td>ISO-like currency code for display/aggregation.</td>
                      <td>Frontend input → <strong>backend normalizes</strong></td>
                      <td>3–5 chars (e.g., <code>USD</code>, <code>GHS</code>). Uppercase enforced server-side.</td>
                      <td>Dropdown or text with suggestions; display uppercase.</td>
                    </tr>
                    <tr>
                      <td><code>source</code></td>
                      <td>Origin/provider (e.g., <code>MOMO</code>, bank name).</td>
                      <td>Frontend input</td>
                      <td>Free text; may be validated against known providers.</td>
                      <td>Small badge beside name helps scanning.</td>
                    </tr>
                    <tr>
                      <td><code>initial_amount</code></td>
                      <td>Starting balance for this wallet at creation.</td>
                      <td>Frontend input → <strong>backend validates</strong></td>
                      <td>&gt;= 0; numeric precision chosen by backend (int cents vs decimal).</td>
                      <td>Editable at creation; changing later should be a specific flow (usually immutable).</td>
                    </tr>
                    <tr>
                      <td><code>balance</code></td>
                      <td>Current computed balance.</td>
                      <td><strong>Backend</strong></td>
                      <td>Derived from transactions; authoritative on refresh.</td>
                      <td><strong>May be updated through the frontend.</strong> Is refreshed after update.</td>
                    </tr>
                    <tr>
                      <td><code>color</code></td>
                      <td>UI accent color (hex).</td>
                      <td>Frontend input → <strong>backend may validate</strong></td>
                      <td>Valid hex; defaults allowed.</td>
                      <td>Color swatch in list item for instant recognition.</td>
                    </tr>
                    <tr>
                      <td><code>created_at</code>, <code>updated_at</code></td>
                      <td>Audit timestamps (ISO strings).</td>
                      <td><strong>Backend</strong></td>
                      <td>Set on insert/update. Server time is the ground truth.</td>
                      <td>Display read-only; helpful in “recent activity” chips.</td>
                    </tr>
                    <tr>
                      <td><code>is_active</code></td>
                      <td>Soft-delete flag (archived vs live).</td>
                      <td><strong>Backend</strong> toggled by actions</td>
                      <td><code>true</code>=live, <code>false</code>=trashed. Restorable.</td>
                      <td>Use “Trash/Restore” actions; never expose as free checkbox.</td>
                    </tr>
                  </tbody>
                </table>
              </div>

              <p><strong>Serialization:</strong> When a wallet is created/updated, the frontend sends a JSON payload with fields like <code>name</code>, <code>currency</code>, <code>source</code>, <code>initial_amount</code>, <code>color</code>. The backend <em>adds</em> authoritative fields (e.g., <code>id</code>, <code>created_at</code>, <code>updated_at</code>, <code>balance</code>, <code>is_active</code>) on write/read. This split keeps the ABI stable and avoids client-side assumptions. During deserialization, C++ maps JSON keys into structs and validates/normalizes before writing to the DB.</p>
            </div>
          </div>
        </article>

        <!-- Transaction -->
        <article class="card bg-base-100 border border-base-300">
          <div class="card-body">
            <h3 class="doc-h3">Transaction</h3>
            <div class="doc-prose">
              <div class="overflow-x-auto">
                <table class="table table-zebra">
                  <thead>
                    <tr>
                      <th>Field</th>
                      <th>What it means</th>
                      <th>Who sets it</th>
                      <th>Constraints &amp; notes</th>
                      <th>UI guidance</th>
                    </tr>
                  </thead>
                  <tbody>
                    <tr>
                      <td><code>id</code></td>
                      <td>Stable identity for the transaction row.</td>
                      <td><strong>Backend</strong></td>
                      <td>Opaque; never reused.</td>
                      <td><strong>Never display or edit.</strong> Selection passes identity internally.</td>
                    </tr>
                    <tr>
                      <td><code>wallet_name</code></td>
                      <td>Wallet the record belongs to (source for income/expense, and sender for transfers).</td>
                      <td>Frontend input → <strong>backend resolves to internal id</strong></td>
                      <td>Must match an existing, active wallet (case/trim rules).</td>
                      <td>Use a picker listing wallets; don’t type raw IDs.</td>
                    </tr>
                    <tr>
                      <td><code>type</code></td>
                      <td>Kind of record.</td>
                      <td>Frontend input</td>
                      <td><code>"income"</code> | <code>"expense"</code> | (for transfer flows, backend may tag internally).</td>
                      <td>Use a select to enforce exact strings for serialization.</td>
                    </tr>
                    <tr>
                      <td><code>category</code></td>
                      <td>Classification (e.g., groceries, transport).</td>
                      <td>Frontend input</td>
                      <td>Free text or taxonomy; normalized server-side if needed.</td>
                      <td>Small badge in list; supports filtering/search.</td>
                    </tr>
                    <tr>
                      <td><code>description</code></td>
                      <td>Optional note/memo.</td>
                      <td>Frontend input</td>
                      <td>Free text; length limits as policy.</td>
                      <td>Multiline input; display truncated with tooltip.</td>
                    </tr>
                    <tr>
                      <td><code>amount</code></td>
                      <td>Transaction amount.</td>
                      <td>Frontend input → <strong>backend validates</strong></td>
                      <td>&gt; 0; precision enforced server-side. Backend applies sign based on <code>type</code>.</td>
                      <td>Numeric input; show currency according to wallet.</td>
                    </tr>
                    <tr>
                      <td><code>related_wallet_name</code></td>
                      <td>Destination wallet for transfers.</td>
                      <td>Frontend input → <strong>backend resolves</strong></td>
                      <td>Required for transfer; must be different from <code>wallet_name</code>.</td>
                      <td>Use a second wallet picker when “Transfer.” Hidden for income/expense.</td>
                    </tr>
                    <tr>
                      <td><code>created_at</code>, <code>updated_at</code></td>
                      <td>Audit timestamps (ISO strings).</td>
                      <td><strong>Backend</strong></td>
                      <td>Set on insert/update; source of truth for ordering.</td>
                      <td>Display read-only; great for “recent” chips or timeline.</td>
                    </tr>
                    <tr>
                      <td><code>is_archived</code></td>
                      <td>Soft-delete/archival flag for records.</td>
                      <td><strong>Backend</strong></td>
                      <td>Consistent with wallet lifecycle.</td>
                      <td>Expose as actions, not free-form checkboxes.</td>
                    </tr>
                  </tbody>
                </table>
              </div>

              <p><strong>Serialization:</strong> For income/expense, JSON includes <code>wallet_name</code>, <code>type</code>, <code>category</code>, <code>description</code>, <code>amount</code>. For transfers, it also includes <code>related_wallet_name</code>. On deserialization, C++ validates fields, resolves names to internal IDs, performs either a single-wallet adjustment (income/expense) or a two-wallet atomic transfer (debit+credit), then returns a boolean. On reads, C++ returns JSON arrays that the frontend parses into <code>Transaction[]</code> for typed rendering.</p>
            </div>
          </div>
        </article>
      </section>

      <!-- ================= WRAPPERS (USAGE + DEEP DIVE) ================= -->

      <!-- create_wallet_wrap -->
      <section id="wrap-create" class="doc-section">
        <h2 class="doc-h2">create_wallet_wrap(<span class="op70 text-xs">to_create: Wallet</span>)</h2>
        <p class="doc-lead">Create a wallet (no IDs in UI). The wrapper shows success/error, refreshes lists, and auto-fades banner.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&lt;div class="grid grid-cols-1 md:grid-cols-3 gap-2"&gt;
  &lt;input class="input input-bordered" placeholder="Name" bind:value=&#123;a_wallet.name&#125; /&gt;
  &lt;input class="input input-bordered" placeholder="Currency" bind:value=&#123;a_wallet.currency&#125; /&gt;
  &lt;input class="input input-bordered" placeholder="Source" bind:value=&#123;a_wallet.source&#125; /&gt;
&lt;/div&gt;

&lt;button class="btn btn-primary mt-3" onclick=&#123;() =&gt; create_wallet_wrap(a_wallet)&#125;&gt;
  Create wallet
&lt;/button&gt;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (event → helper → Saucer → C++ → UI)</summary>
          <div class="doc-prose">
            <p><strong>Analogy:</strong> the wrapper is a front-desk clerk…</p>
            <p><strong>Why wrapper:</strong> concentrate UX boilerplate: call <code>create_wallet()</code> → map boolean to <code>&#123; success_popup, error_popup &#125;</code> → update UI → <code>refresh_wrap()</code> → <code>fade_alert()</code>.</p>
            <p><strong>Why no IDs in UI:</strong> IDs are backend implementation details…</p>
            <p><strong>Serialization:</strong> helper packs JSON to cross the JS↔C++ boundary safely.</p>
            <p><strong>Boundary + return:</strong> <code>window.saucer.exposed.create_wallet(payload: string) ⇒ Promise&lt;boolean&gt;</code>…</p>
            <p><strong>C++ path:</strong> parse, validate, persist, stamp <code>created_at/updated_at</code>, return boolean.</p>
          </div>
        </details>
      </section>

      <!-- record_transaction_wrap -->
      <section id="wrap-record" class="doc-section">
        <h2 class="doc-h2">record_transaction_wrap(<span class="op70 text-xs">to_record: Transaction</span>)</h2>
        <p class="doc-lead">Record an income/expense against a wallet. Backend applies the math; UI never shows IDs.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&lt;div class="grid grid-cols-1 md:grid-cols-3 gap-2"&gt;
  &lt;input class="input input-bordered" type="number" placeholder="Amount" bind:value=&#123;a_record.amount&#125; /&gt;
  &lt;select class="select select-bordered" bind:value=&#123;a_record.type&#125;&gt;
    &lt;option value="income"&gt;income&lt;/option&gt;
    &lt;option value="expense"&gt;expense&lt;/option&gt;
  &lt;/select&gt;
  &lt;input class="input input-bordered" placeholder="Category" bind:value=&#123;a_record.category&#125; /&gt;
&lt;/div&gt;

&lt;button class="btn mt-3" onclick=&#123;() =&gt; record_transaction_wrap(a_record)&#125;&gt;
  Record transaction
&lt;/button&gt;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (flow + design choices)</summary>
          <div class="doc-prose">
            <p><strong>Analogy:</strong> drop a slip into the ledger…</p>
            <p><strong>Why server math:</strong> one source of truth…</p>
            <p><strong>Backend:</strong> verify, insert, adjust balance atomically; boolean back; wrapper maps to popups + refresh + fade.</p>
          </div>
        </details>
      </section>

      <!-- record_transfer_wrap -->
      <section id="wrap-transfer" class="doc-section">
        <h2 class="doc-h2">record_transfer_wrap(<span class="op70 text-xs">to_record: Transaction</span>)</h2>
        <p class="doc-lead">Move funds between wallets — source <code>wallet_name</code> → destination <code>related_wallet_name</code>. One atomic backend operation.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&lt;div class="grid grid-cols-1 md:grid-cols-3 gap-2"&gt;
  &lt;input class="input input-bordered" placeholder="From (wallet_name)" bind:value=&#123;a_record.wallet_name&#125; /&gt;
  &lt;input class="input input-bordered" placeholder="To (related_wallet_name)" bind:value=&#123;a_record.related_wallet_name&#125; /&gt;
  &lt;input class="input input-bordered" type="number" placeholder="Amount" bind:value=&#123;a_record.amount&#125; /&gt;
&lt;/div&gt;

&lt;button class="btn mt-3" onclick=&#123;() =&gt; record_transfer_wrap(a_record)&#125;&gt;
  Record transfer
&lt;/button&gt;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (atomicity &amp; integrity)</summary>
          <div class="doc-prose">
            <p><strong>One operation, two effects:</strong> debit A + credit B inside a DB transaction; no partial states. JSON payload is explicit and evolvable.</p>
          </div>
        </details>
      </section>

      <!-- update_transaction_wrap -->
      <section id="wrap-upd-tx" class="doc-section">
        <h2 class="doc-h2">update_transaction_wrap(<span class="op70 text-xs">to_update: Transaction</span>)</h2>
        <p class="doc-lead">Edit a transaction you selected from a list. Backend re-diffs old vs new and adjusts balances.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&lt;!-- Show a human index (never the DB id) --&gt;
&#123;#each fetched_transactions as t, i (t.id)&#125;
  &lt;div class="card bg-base-100 border border-base-300 mb-2 p-3"&gt;
    &lt;div class="flex items-center justify-between"&gt;
      &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;t.category&#125; • &#123;t.amount&#125;&lt;/div&gt;
      &lt;button class="btn btn-accent btn-sm" onclick=&#123;() =&gt; update_transaction_wrap(t)&#125;&gt;Edit&lt;/button&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (re-diffing &amp; correctness)</summary>
          <div class="doc-prose">
            <p><strong>Why server re-diff:</strong> reverse old effect + apply new, respecting policy and concurrency. Identity is internal; IDs are never shown or edited.</p>
          </div>
        </details>
      </section>

      <!-- update_transfer_wrap -->
      <section id="wrap-upd-tr" class="doc-section">
        <h2 class="doc-h2">update_transfer_wrap(<span class="op70 text-xs">to_update: Transaction</span>)</h2>
        <p class="doc-lead">Edit a transfer. Backend updates both ends atomically and keeps totals coherent.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_transactions.filter(t =&gt; t.type === "transfer") as t, i (t.id)&#125;
  &lt;div class="card bg-base-100 border border-base-300 mb-2 p-3"&gt;
    &lt;div class="flex items-center justify-between"&gt;
      &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;t.wallet_name&#125; → &#123;t.related_wallet_name&#125; • &#123;t.amount&#125;&lt;/div&gt;
      &lt;button class="btn btn-accent btn-sm" onclick=&#123;() =&gt; update_transfer_wrap(t)&#125;&gt;Edit transfer&lt;/button&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (two-sided consistency)</summary>
          <div class="doc-prose">
            <p>Backend loads previous transfer, computes both deltas, applies them in one DB transaction, or rolls back on failure. UI stays simple.</p>
          </div>
        </details>
      </section>

      <!-- update_wallet_wrap -->
      <section id="wrap-upd-wa" class="doc-section">
        <h2 class="doc-h2">update_wallet_wrap(<span class="op70 text-xs">to_update: Wallet</span>)</h2>
        <p class="doc-lead">Edit wallet properties (never ID). Timestamps are read-only, authored by the backend.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_wallets as w, i (w.id)&#125;
  &lt;div class="card bg-base-100 border border-base-300 mb-2 p-3"&gt;
    &lt;div class="flex flex-wrap items-center gap-2 justify-between"&gt;
      &lt;div class="flex items-center gap-2"&gt;
        &lt;span class="badge"&gt;#&#123;i + 1&#125;&lt;/span&gt;
        &lt;span class="font-semibold"&gt;&#123;w.name&#125;&lt;/span&gt;
        &lt;span class="opacity-70 text-xs"&gt;updated: &#123;w.updated_at || "—"&#125;&lt;/span&gt;
      &lt;/div&gt;
      &lt;button class="btn btn-accent btn-sm" onclick=&#123;() =&gt; update_wallet_wrap(w)&#125;&gt;Edit&lt;/button&gt;
    &lt;/div&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (normalization &amp; consistency)</summary>
          <div class="doc-prose">
            <p>Server normalizes (e.g., uppercase currency, validate hex). Refresh displays exactly what the server accepted.</p>
          </div>
        </details>
      </section>

      <!-- restore_wallet_wrap -->
      <section id="wrap-restore" class="doc-section">
        <h2 class="doc-h2">restore_wallet_wrap(<span class="op70 text-xs">to_restore: Wallet</span>)</h2>
        <p class="doc-lead">Bring a trashed wallet back. Selection, not ID; same wrapper rhythm (call, refresh, fade).</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_wallets.filter(w =&gt; !w.is_active) as w, i (w.id)&#125;
  &lt;div class="flex items-center justify-between mb-2 p-3 border rounded-xl bg-base-100"&gt;
    &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;w.name&#125; (archived)&lt;/div&gt;
    &lt;button class="btn btn-warning btn-sm" onclick=&#123;() =&gt; restore_wallet_wrap(w)&#125;&gt;Restore&lt;/button&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (lifecycle)</summary>
          <div class="doc-prose">
            <p>Soft delete → <code>is_active=false</code>. Restore flips it back after validation (e.g., name uniqueness). Same boolean verdict/pattern as other mutations.</p>
          </div>
        </details>
      </section>

      <!-- trash_wallet_wrap -->
      <section id="wrap-trash" class="doc-section">
        <h2 class="doc-h2">trash_wallet_wrap(<span class="op70 text-xs">to_trash: Wallet</span>)</h2>
        <p class="doc-lead">Archive a wallet (reversible). Permanent removal is a different wrapper.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_wallets.filter(w =&gt; w.is_active) as w, i (w.id)&#125;
  &lt;div class="flex items-center justify-between mb-2 p-3 border rounded-xl bg-base-100"&gt;
    &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;w.name&#125;&lt;/div&gt;
    &lt;button class="btn btn-warning btn-sm" onclick=&#123;() =&gt; trash_wallet_wrap(w)&#125;&gt;Trash&lt;/button&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (why soft delete)</summary>
          <div class="doc-prose">
            <p>Reversible and auditable lifecycle. Keeps UI cadence identical to other mutations.</p>
          </div>
        </details>
      </section>

      <!-- delete_wallet_wrap -->
      <section id="wrap-del-wa" class="doc-section">
        <h2 class="doc-h2">delete_wallet_wrap(<span class="op70 text-xs">to_delete: Wallet</span>)</h2>
        <p class="doc-lead"><strong>Permanent</strong> delete. Confirm with a modal. Backend may enforce referential integrity.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_wallets as w, i (w.id)&#125;
  &lt;div class="flex items-center justify-between mb-2 p-3 border rounded-xl bg-base-100"&gt;
    &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;w.name&#125;&lt;/div&gt;
    &lt;button class="btn btn-error btn-sm" onclick=&#123;() =&gt; delete_wallet_wrap(w)&#125;&gt;Delete&lt;/button&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (irreversibility)</summary>
          <div class="doc-prose">
            <p>Deletes may be blocked/cascaded by policy; server enforces it. Boolean verdict → refresh to rehydrate lists.</p>
          </div>
        </details>
      </section>

      <!-- delete_record_wrap -->
      <section id="wrap-del-tx" class="doc-section">
        <h2 class="doc-h2">delete_record_wrap(<span class="op70 text-xs">to_delete: Transaction</span>)</h2>
        <p class="doc-lead">Remove a transaction and reverse its balance effect on the backend.</p>

        <div class="codecard card bg-base-200 shadow-xl tilt3d">
          <div class="card-body p-0">
            <div class="codebar">
              <span>HTML (Svelte&nbsp;5)</span>
              <button class="btn btn-xs btn-ghost" onclick={handleCopyClick}>Copy</button>
            </div>
            <div class="mockup-code text-xs">
<pre><code>&#123;#each fetched_transactions as t, i (t.id)&#125;
  &lt;div class="flex items-center justify-between mb-2 p-3 border rounded-xl bg-base-100"&gt;
    &lt;div&gt;&lt;span class="badge mr-2"&gt;#&#123;i + 1&#125;&lt;/span&gt; &#123;t.category&#125; • &#123;t.amount&#125;&lt;/div&gt;
    &lt;button class="btn btn-error btn-sm" onclick=&#123;() =&gt; delete_record_wrap(t)&#125;&gt;Delete&lt;/button&gt;
  &lt;/div&gt;
&#123;/each&#125;</code></pre>
            </div>
          </div>
        </div>

        <details class="mt-4">
          <summary class="doc-summary">Deep explanation (reversal)</summary>
          <div class="doc-prose">
            <p>Server reverses the prior balance effect then deletes the row. Guarantees ledger correctness across time.</p>
          </div>
        </details>
      </section>

      <!-- refresh_wrap -->
      <section id="wrap-refresh" class="doc-section">
        <h2 class="doc-h2">refresh_wrap()</h2>
        <p class="doc-lead">Fetch <code>&#123; wallets, records &#125;</code> in one shot after any mutation, so UI mirrors the ledger.</p>
        <details class="mt-2">
          <summary class="doc-summary">Deep explanation (why batched)</summary>
          <div class="doc-prose">
            <p>Prevents “half updated” screens and ensures computed values (balances, timestamps) are consistent. Future lists can be added centrally.</p>
          </div>
        </details>
      </section>

      <!-- get_*_wrap + fade_alert -->
      <section id="wrap-gets" class="doc-section">
        <h2 class="doc-h2">get_wallets_wrap() &amp; get_records_wrap()</h2>
        <p class="doc-lead">Targeted reads for one list at a time. <strong>No usage snippets here</strong> (as requested).</p>
        <details class="mt-2">
          <summary class="doc-summary">Deep explanation</summary>
          <div class="doc-prose">
            <p>When only one pane needs reloading, these wrappers keep feedback (empty/error strings) consistent with mutations.</p>
          </div>
        </details>
      </section>

      <section id="wrap-fade" class="doc-section">
        <h2 class="doc-h2">fade_alert()</h2>
        <p class="doc-lead">Auto-dismiss success/error banners after ~3s for a calmer interface.</p>
        <details class="mt-2">
          <summary class="doc-summary">Deep explanation</summary>
          <div class="doc-prose">
            <p>Prevents stale banners and standardizes the completion feel of every mutation.</p>
          </div>
        </details>
      </section>

      <!-- ================== SERIALIZATION / DESERIALIZATION ================== -->
      <section id="serde" class="doc-section">
        <h2 class="doc-h2">Serialization &amp; Deserialization — the border language</h2>
        <div class="card bg-base-200 shadow">
          <div class="card-body doc-prose">
            <p><strong>Serialization</strong> turns JS objects into JSON so they can cross into C++. <strong>Deserialization</strong> reconstructs C++ structs. JSON is loggable, diffable, resilient to field order, and avoids ABI pitfalls. Old servers ignore new fields; new servers default old payloads — compatibility by design.</p>
            <p><strong>Why boolean results:</strong> simple, uniform UI branches. Helpers map boolean → <code>&#123; success_popup, error_popup &#125;</code>. If you need rich errors later, return <code>&#123; ok, code, message &#125;</code> from C++ and adapt inside helpers without touching the UI.</p>
          </div>
        </div>
      </section>

      <!-- ================== INDEX.TS HELPERS: DEEP EXPLANATIONS ONLY ================== -->
      <section id="helpers" class="doc-section">
        <h2 class="doc-h2">Core helpers in <code>$lib/index.ts</code> — deep explanations</h2>

        <article id="idx-refresh" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">refresh()</h3>
            <div class="doc-prose">
              <p>Aggregates <code>get_wallets()</code> and <code>get_records()</code> so post-mutation code is predictable: “always refresh both.” If you add lists later, extend <code>refresh()</code> once and all callers benefit.</p>
            </div>
          </div>
        </article>

        <article id="idx-create" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">create_wallet(to_create: Wallet)</h3>
            <div class="doc-prose">
              <p>Stringify → call <code>window.saucer.exposed.create_wallet</code> → map boolean to popups. Server sets timestamps + ID. Frontend never shows/edits IDs.</p>
            </div>
          </div>
        </article>

        <article id="idx-record" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">record_transaction(to_record: Transaction)</h3>
            <div class="doc-prose">
              <p>Validate wallet/type/amount server-side; insert; adjust balance atomically. Helper returns popup strings; wrappers stay uniform.</p>
            </div>
          </div>
        </article>

        <article id="idx-transfer" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">record_transfer(to_record: Transaction)</h3>
            <div class="doc-prose">
              <p>Debit + credit inside one DB transaction to prevent partial states; JSON payload is explicit and evolvable.</p>
            </div>
          </div>
        </article>

        <article id="idx-upd-transfer" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">update_transfer(to_update: Transaction)</h3>
            <div class="doc-prose">
              <p>Server re-diffs and rebalances both wallets atomically. Boolean verdict; refresh shows canonical results.</p>
            </div>
          </div>
        </article>

        <article id="idx-upd-record" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">update_record(to_update: Transaction)</h3>
            <div class="doc-prose">
              <p>Re-diff and persist server-side; stamp <code>updated_at</code>. Helpers keep popup shape identical.</p>
            </div>
          </div>
        </article>

        <article id="idx-upd-wallet" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">update_wallet(to_update: Wallet)</h3>
            <div class="doc-prose">
              <p>Normalize/validate (e.g., color hex, unique names) on the server; return boolean; UI shows timestamps read-only.</p>
            </div>
          </div>
        </article>

        <article id="idx-restore" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">restore_wallet(to_restore: Wallet)</h3>
            <div class="doc-prose">
              <p>Lifecycle flip to active; same contract as other mutations for consistency and testability.</p>
            </div>
          </div>
        </article>

        <article id="idx-get-wallets" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">get_wallets()</h3>
            <div class="doc-prose">
              <p>C++ returns JSON string; helper parses into <code>Wallet[]</code> at the UI edge for type safety.</p>
            </div>
          </div>
        </article>

        <article id="idx-get-records" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">get_records()</h3>
            <div class="doc-prose">
              <p>Same pattern → parse to <code>Transaction[]</code>. Compact, explicit transport.</p>
            </div>
          </div>
        </article>

        <article id="idx-trash" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">trash_wallet(to_trash: Wallet)</h3>
            <div class="doc-prose">
              <p>Calls the soft-delete C++ binding (exposed as <code>delete_wallet</code>). Helper name “trash” avoids confusing it with permanent deletion.</p>
            </div>
          </div>
        </article>

        <article id="idx-del-wallet" class="card bg-base-100 border border-base-300 mb-4">
          <div class="card-body">
            <h3 class="doc-h3">delete_wallet(to_delete: Wallet)</h3>
            <div class="doc-prose">
              <p>Permanent removal via <code>delete_wallet_permanently</code>. Integrity rules enforced server-side.</p>
            </div>
          </div>
        </article>

        <article id="idx-del-record" class="card bg-base-100 border border-base-300">
          <div class="card-body">
            <h3 class="doc-h3">delete_record(to_delete: Transaction)</h3>
            <div class="doc-prose">
              <p>Find by internal id, reverse balance effect, delete row, return boolean. UI never exposes the id.</p>
            </div>
          </div>
        </article>
      </section>

      <!-- Backend contract -->
      <section id="backend" class="doc-section">
        <h2 class="doc-h2">Backend contract (Saucer → C++)</h2>
        <div class="card bg-base-200 shadow">
          <div class="card-body doc-prose">
            <ul>
              <li><strong>Writes:</strong> <code>(payload: string) =&gt; Promise&lt;boolean&gt;</code>.</li>
              <li><strong>Reads:</strong> <code>() =&gt; Promise&lt;string&gt;</code> (JSON arrays).</li>
              <li><strong>IDs:</strong> backend-generated; never shown/edited in UI.</li>
              <li><strong>Timestamps:</strong> backend-authored; display OK; editing not allowed.</li>
              <li><strong>Atomicity:</strong> transfers/updates use DB transactions to preserve invariants.</li>
              <li><strong>Evolvability:</strong> unknown JSON fields ignored; optional fields can be added safely.</li>
            </ul>
          </div>
        </div>
      </section>

      <!-- ================== FLOW CHART (OPTIONAL) ================== -->
      <section id="flow" class="doc-section">
        <h2 class="doc-h2">How everything flows (high-level)</h2>
        <div class="card bg-base-100 border border-base-300">
          <div class="card-body">
            <div class="doc-prose">
              <p>From user actions to durable state and back to UI. The diagram emphasizes serialization at the boundary and why IDs/timestamps are backend authored.</p>
            </div>
            <!-- Accessible, responsive SVG -->
            <div class="w-full overflow-auto">
              <svg role="img" aria-label="Application flow" viewBox="0 0 1200 520" class="max-w-full">
                <defs>
                  <style>
                    .n { fill: #fff; stroke: #cbd5e1; stroke-width: 2; rx: 10; ry: 10; }
                    .t { font: 14px ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, Helvetica, Arial; fill: #0f172a; }
                    .a { stroke: #334155; stroke-width: 2; marker-end: url(#arrow); }
                  </style>
                  <marker id="arrow" markerWidth="10" markerHeight="8" refX="10" refY="4" orient="auto">
                    <polygon points="0,0 10,4 0,8" fill="#334155" />
                  </marker>
                </defs>

                <!-- Row 1: UI -->
                <rect class="n" x="40" y="40" width="320" height="80" />
                <text class="t" x="60" y="75">Svelte UI (wrappers)</text>
                <text class="t" x="60" y="98">onclick → wrapper orchestration</text>

                <rect class="n" x="420" y="40" width="320" height="80" />
                <text class="t" x="440" y="75">$lib/index.ts (helpers)</text>
                <text class="t" x="440" y="98">serialize payload → call Saucer</text>

                <rect class="n" x="800" y="40" width="360" height="80" />
                <text class="t" x="820" y="75">Saucer bridge</text>
                <text class="t" x="820" y="98">JS ↔ C++ boundary</text>

                <line class="a" x1="360" y1="80" x2="420" y2="80"/>
                <line class="a" x1="740" y1="80" x2="800" y2="80"/>

                <!-- Row 2: Backend -->
                <rect class="n" x="40" y="190" width="360" height="100" />
                <text class="t" x="60" y="225">C++ handlers</text>
                <text class="t" x="60" y="248">deserialize JSON → validate → act</text>

                <rect class="n" x="440" y="190" width="320" height="100" />
                <text class="t" x="460" y="225">DB (wallets, transactions)</text>
                <text class="t" x="460" y="248">IDs/timestamps/balances authored here</text>

                <line class="a" x1="980" y1="120" x2="220" y2="190"/>
                <line class="a" x1="400" y1="240" x2="440" y2="240"/>
                <line class="a" x1="760" y1="240" x2="880" y2="240"/>

                <!-- Row 3: Response -->
                <rect class="n" x="800" y="330" width="360" height="100" />
                <text class="t" x="820" y="365">C++ → Saucer → JS</text>
                <text class="t" x="820" y="388">boolean for writes; JSON strings for reads</text>

                <rect class="n" x="420" y="330" width="320" height="100" />
                <text class="t" x="440" y="365">helpers parse JSON</text>
                <text class="t" x="440" y="388">map boolean → &#123; success_popup, error_popup &#125;</text>

                <rect class="n" x="40" y="330" width="320" height="100" />
                <text class="t" x="60" y="365">wrappers update state</text>
                <text class="t" x="60" y="388">refresh → fade_alert → UI shows truth</text>

                <line class="a" x1="980" y1="380" x2="740" y2="380"/>
                <line class="a" x1="420" y1="380" x2="360" y2="380"/>
              </svg>
            </div>
          </div>
        </div>
      </section>
    </main>

    <!-- ======================= RIGHT TOC (sticky, high z-index) ======================= -->
    <aside class="hidden xl:block">
      <div class="sticky top-[96px] z-[6500]">
        <div class="card bg-base-200 shadow border border-base-300">
          <div class="card-body p-4">
            <h3 class="font-semibold mb-2">On this page</h3>
            <ul class="menu text-sm">
              <li><a href="#guardrails">Guardrails</a></li>
              <li><a href="#field-reference">Field Reference</a></li>
              <li class="menu-title opacity-60">Wrappers</li>
              <li><a href="#wrap-create">create_wallet_wrap</a></li>
              <li><a href="#wrap-record">record_transaction_wrap</a></li>
              <li><a href="#wrap-transfer">record_transfer_wrap</a></li>
              <li><a href="#wrap-upd-tx">update_transaction_wrap</a></li>
              <li><a href="#wrap-upd-tr">update_transfer_wrap</a></li>
              <li><a href="#wrap-upd-wa">update_wallet_wrap</a></li>
              <li><a href="#wrap-restore">restore_wallet_wrap</a></li>
              <li><a href="#wrap-trash">trash_wallet_wrap</a></li>
              <li><a href="#wrap-del-wa">delete_wallet_wrap</a></li>
              <li><a href="#wrap-del-tx">delete_record_wrap</a></li>
              <li><a href="#wrap-refresh">refresh_wrap</a></li>
              <li><a href="#wrap-gets">get_*_wrap</a></li>
              <li><a href="#wrap-fade">fade_alert</a></li>
              <li class="menu-title opacity-60">How the border works</li>
              <li><a href="#serde">Serialization/Deserialization</a></li>
              <li class="menu-title opacity-60">$lib/index.ts</li>
              <li><a href="#idx-refresh">refresh()</a></li>
              <li><a href="#idx-create">create_wallet()</a></li>
              <li><a href="#idx-record">record_transaction()</a></li>
              <li><a href="#idx-transfer">record_transfer()</a></li>
              <li><a href="#idx-upd-transfer">update_transfer()</a></li>
              <li><a href="#idx-upd-record">update_record()</a></li>
              <li><a href="#idx-upd-wallet">update_wallet()</a></li>
              <li><a href="#idx-restore">restore_wallet()</a></li>
              <li><a href="#idx-get-wallets">get_wallets()</a></li>
              <li><a href="#idx-get-records">get_records()</a></li>
              <li><a href="#idx-trash">trash_wallet()</a></li>
              <li><a href="#idx-del-wallet">delete_wallet()</a></li>
              <li><a href="#idx-del-record">delete_record()</a></li>
              <li class="menu-title opacity-60">Backend</li>
              <li><a href="#backend">Contract</a></li>
              <li><a href="#flow">Flow chart</a></li>
            </ul>
          </div>
        </div>
      </div>
    </aside>
  </div>

  <!-- Mobile TOC launcher (kept above everything) -->
  <button
    class="xl:hidden btn btn-circle btn-primary fixed bottom-5 right-5 shadow-lg z-[7000]"
    aria-label="Open table of contents"
    onclick={() => (showTOC = true)}
  >☰</button>

  <!-- Mobile TOC modal -->
  {#if showTOC}
    <div class="modal modal-open z-[8000]">
      <div class="modal-box">
        <h3 class="font-bold text-lg mb-2">On this page</h3>
        <ul class="menu text-sm max-h-[60vh] overflow-auto">
          <li><a href="#guardrails" onclick={() => (showTOC = false)}>Guardrails</a></li>
          <li><a href="#field-reference" onclick={() => (showTOC = false)}>Field Reference</a></li>
          <li class="menu-title opacity-60">Wrappers</li>
          <li><a href="#wrap-create" onclick={() => (showTOC = false)}>create_wallet_wrap</a></li>
          <li><a href="#wrap-record" onclick={() => (showTOC = false)}>record_transaction_wrap</a></li>
          <li><a href="#wrap-transfer" onclick={() => (showTOC = false)}>record_transfer_wrap</a></li>
          <li><a href="#wrap-upd-tx" onclick={() => (showTOC = false)}>update_transaction_wrap</a></li>
          <li><a href="#wrap-upd-tr" onclick={() => (showTOC = false)}>update_transfer_wrap</a></li>
          <li><a href="#wrap-upd-wa" onclick={() => (showTOC = false)}>update_wallet_wrap</a></li>
          <li><a href="#wrap-restore" onclick={() => (showTOC = false)}>restore_wallet_wrap</a></li>
          <li><a href="#wrap-trash" onclick={() => (showTOC = false)}>trash_wallet_wrap</a></li>
          <li><a href="#wrap-del-wa" onclick={() => (showTOC = false)}>delete_wallet_wrap</a></li>
          <li><a href="#wrap-del-tx" onclick={() => (showTOC = false)}>delete_record_wrap</a></li>
          <li><a href="#wrap-refresh" onclick={() => (showTOC = false)}>refresh_wrap</a></li>
          <li><a href="#wrap-gets" onclick={() => (showTOC = false)}>get_*_wrap</a></li>
          <li><a href="#wrap-fade" onclick={() => (showTOC = false)}>fade_alert</a></li>
          <li class="menu-title opacity-60">How the border works</li>
          <li><a href="#serde" onclick={() => (showTOC = false)}>Serialization/Deserialization</a></li>
          <li class="menu-title opacity-60">$lib/index.ts</li>
          <li><a href="#idx-refresh" onclick={() => (showTOC = false)}>refresh()</a></li>
          <li><a href="#idx-create" onclick={() => (showTOC = false)}>create_wallet()</a></li>
          <li><a href="#idx-record" onclick={() => (showTOC = false)}>record_transaction()</a></li>
          <li><a href="#idx-transfer" onclick={() => (showTOC = false)}>record_transfer()</a></li>
          <li><a href="#idx-upd-transfer" onclick={() => (showTOC = false)}>update_transfer()</a></li>
          <li><a href="#idx-upd-record" onclick={() => (showTOC = false)}>update_record()</a></li>
          <li><a href="#idx-upd-wallet" onclick={() => (showTOC = false)}>update_wallet()</a></li>
          <li><a href="#idx-restore" onclick={() => (showTOC = false)}>restore_wallet()</a></li>
          <li><a href="#idx-get-wallets" onclick={() => (showTOC = false)}>get_wallets()</a></li>
          <li><a href="#idx-get-records" onclick={() => (showTOC = false)}>get_records()</a></li>
          <li><a href="#idx-trash" onclick={() => (showTOC = false)}>trash_wallet()</a></li>
          <li><a href="#idx-del-wallet" onclick={() => (showTOC = false)}>delete_wallet()</a></li>
          <li><a href="#idx-del-record" onclick={() => (showTOC = false)}>delete_record()</a></li>
          <li class="menu-title opacity-60">Backend</li>
          <li><a href="#backend" onclick={() => (showTOC = false)}>Contract</a></li>
          <li><a href="#flow" onclick={() => (showTOC = false)}>Flow chart</a></li>
        </ul>
        <div class="modal-action">
          <button class="btn" onclick={() => (showTOC = false)}>Close</button>
        </div>
      </div>
    </div>
  {/if}
</div>

<style>
  /* Smooth scroll + avoid navbar overlap (works with your fixed layout header) */
  :global(html) { scroll-behavior: smooth; }
  :global(:target) { scroll-margin-top: 96px; }

  /* Headings & intro */
  :global(.docs-title) {
    font-size: clamp(1.75rem, 1.2rem + 1.8vw, 2.5rem);
    font-weight: 900;
    letter-spacing: -0.02em;
    background: linear-gradient(90deg, hsl(var(--p, 262 83% 58%)), hsl(var(--s, 199 89% 48%)));
    -webkit-background-clip: text;
    background-clip: text;
    color: transparent;
  }
  :global(.docs-subtitle) { margin-top: .5rem; max-width: 70ch; opacity: .85; line-height: 1.7; }
  :global(.docs-hero) { position: relative; padding: .5rem 0; }

  /* Sections */
  :global(.doc-section) { scroll-margin-top: 110px; }
  :global(.doc-h2) { font-weight: 900; font-size: clamp(1.25rem, 1rem + 1vw, 1.75rem); letter-spacing: -0.01em; margin-bottom: .5rem; }
  :global(.doc-h3) { font-weight: 800; font-size: 1.05rem; }

  /* Readable “prose” spacing */
  :global(.doc-prose p) { margin: 0 0 1rem 0; line-height: 1.85; }
  :global(.doc-prose ul), :global(.doc-prose ol) { margin: 0 0 1rem 1.25rem; line-height: 1.8; }
  :global(.doc-prose li + li) { margin-top: .35rem; }
  :global(.doc-lead) { opacity: .9; margin-bottom: .5rem; line-height: 1.8; }
  :global(.doc-summary) { cursor: pointer; font-weight: 700; }

  /* Code blocks — explicit colors and backgrounds to always be readable */
  :global(.codebar) {
    display: flex; align-items: center; justify-content: space-between;
    padding: .55rem .75rem; font-size: .8rem;
    border-bottom: 1px solid hsl(var(--b3, 210 20% 85%));
    background: hsl(var(--b2, 0 0% 96%));
    color: hsl(var(--bc, 222 47% 11%));
  }
  :global(.codebar .btn) { color: hsl(var(--bc, 222 47% 11%)); }

  :global(.mockup-code) {
    background: hsl(var(--b1, 0 0% 100%));
    color: hsl(var(--bc, 222 47% 11%));
    border-top: 1px solid hsl(var(--b3, 210 20% 85%));
    border-bottom-left-radius: .5rem;
    border-bottom-right-radius: .5rem;
  }
  :global(.mockup-code pre) { overflow-x: auto; padding: 1rem 1.25rem; margin: 0; background: inherit; color: inherit; }
  :global(.mockup-code code) {
    font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, "Liberation Mono", "Courier New", monospace;
    font-size: .9rem; line-height: 1.6; color: hsl(var(--bc, 222 47% 11%)); white-space: pre;
  }

  /* Subtle 3D tilt on hover */
  :global(.tilt3d) { transform-style: preserve-3d; transition: transform .25s ease; will-change: transform; }
  :global(.tilt3d:hover) { transform: perspective(1000px) rotateX(1.2deg) rotateY(-1.2deg) translateZ(2px); }

  /* Soft fade for hero */
  :global(.fx-fade) { animation: fx-fade .6s ease both; }
  @keyframes fx-fade { from { opacity: 0; transform: translateY(6px) } to { opacity: 1; transform: translateY(0) } }

  /* Ensure readable text even without theme; keep components inheriting that color */
  :global(.docs-root) { color: hsl(var(--bc, 222 47% 11%)); background: hsl(var(--b1, 0 0% 100%)); }
  :global(.card), :global(.modal-box) { color: inherit; background: hsl(var(--b1, 0 0% 100%)); }

  /* TOC: keep above page content; below your navbar (which uses a very high z) */
  :global(aside .sticky) { z-index: 6500; }
</style>
