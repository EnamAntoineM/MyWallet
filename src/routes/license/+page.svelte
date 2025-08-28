<script lang="ts">
    let appName = "WALLET TRACKER";
    let licenseType = "MIT License";
    let copyrightYear = "2025";
    let companyName = "Enam A.-M. KODJOH-KPAKPASSOU";

    let licenseText = `
    Permission is hereby granted to any person obtaining a copy of this software and associated 
    documentation files to use this software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
  `;
</script>

<svelte:head>
    <title>License - {appName}</title>
</svelte:head>

<div class="max-w-4xl mx-auto p-8 bg-white rounded-lg shadow-lg my-6">
    <header class="text-center border-b border-gray-200 pb-6 mb-8">
        <h1 class="text-4xl font-extrabold text-yellow-500 focus-text-yellow-900 mb-2">{licenseType}</h1>
    </header>

    <main>
        <div class="license-content">
            <p class="font-semibold text-gray-800 text-center mb-8">
                Copyright © {copyrightYear}
                {companyName}. All rights reserved.
            </p>

            <div
                class="bg-gray-100 p-6 rounded-md border-l-4 border-yellow-500 mb-8 whitespace-pre-wrap"
            >
                <p class="text-gray-700 text-2x1 font-medium text-justify">
                    {licenseText}
                </p>
            </div>
        </div>
    </main>
    <footer
        class="text-center border-t border-gray-200 pt-6 text-blue-400 font-semibold"
    >
        <p class="mb-1 text-yellow-600 uppercase not-italic font-bold">Contact us</p>
        <div class="flex items-center gap-4 justify-center">
            <a
                href="mailto:eakodjoh-kpakpassou@st.ug.edu.gh"
                class="flex items-center gap-2 hover:text-blue-700"
            >
                <i class="fa-solid fa-envelope"></i>
                eakodjoh-kpakpassou@st.ug.edu.gh
            </a>
            <a
                href="https://www.linkedin.com/in/enam-kodjoh-kpakpassou"
                target="_blank"
                rel="noopener"
                class="flex items-center gap-2 hover:text-blue-700"
            >
                <i class="fa-brands fa-linkedin"></i>
                LinkedIn
            </a>
        </div>
    </footer>
</div>
