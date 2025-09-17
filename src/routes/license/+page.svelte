<script lang="ts">
	let licenseName = "MIT License";
	let copyrightYear = "2025";
	let companyName = "Enam A.-M. KODJOH-KPAKPASSOU";

	let licenseText = `
  Permission is hereby granted to any person obtaining a copy of this software and associated documentation files to use this software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.`;
</script>

<div
	class="flex w-full bg-gradient-to-br from-base-200 to-base-content/10 min-h-screen justify-center items-center flex-col lg:pt-17 2xl:pt-35"
>
	<div
		class="flex max-w-3xl flex-fit w-full mx-auto p-6 bg-base-100 rounded-lg shadow-2xl my-6 flex flex-col"
	>
		<header class="text-center border-b border-base-100 pt-6 pb-6 mb-8">
			<h1
				class="text-4xl font-extrabold text-warning mb-2"
			>
				{licenseName}
			</h1>
		</header>

		<div class="flex-1 pb-8">
			<p class="font-semibold text-base-content text-center mb-3">
				© {copyrightYear}
				{companyName}. All rights reserved.
			</p>

			<div
				class="bg-base-100 pb-6 pl-8 pr-8 rounded-l-xl shadow-md rounded-r-2xl border-l-4 border-warning whitespace-pre-wrap"
			>
				<p
					class="text-base-content sm:text-lg font-medium text-justify leading-relaxed"
				>
					{licenseText}
				</p>
			</div>
		</div>
	</div>
</div>
