<script lang="ts">
	let licenseName = "MIT License";
	let copyrightYear = "2025";
	let companyName = "Enam A.-M. KODJOH-KPAKPASSOU";

	let licenseText = `
  Permission is hereby granted to any person obtaining a copy of this software and associated documentation files to use this software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the software. 

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.`;
</script>

<div
	class="flex w-full min-h-full justify-center items-center flex-col lg:pt-17 2xl:pt-35"
>
	<div
		class="flex-1 max-w-3xl w-full mx-auto p-6 bg-white rounded-lg shadow-xl my-6 flex flex-col"
	>
		<header class="text-center border-b border-gray-200 pt-6 pb-6 mb-8">
			<h1
				class="text-4xl font-extrabold text-yellow-500 focus:text-yellow-900 mb-2"
			>
				{licenseName}
			</h1>
		</header>

		<div class="flex-1 pb-8">
			<p class="font-semibold text-gray-800 text-center mb-3">
				© {copyrightYear}
				{companyName}. All rights reserved.
			</p>

			<div
				class="bg-gray-100 pb-6 pl-8 pr-8 rounded-l-xl shadow-md rounded-r-2xl border-l-4 border-yellow-500 whitespace-pre-wrap"
			>
				<p
					class="text-gray-700 text-base sm:text-lg font-medium text-justify leading-relaxed"
				>
					{licenseText}
				</p>
			</div>
		</div>
	</div>
</div>
