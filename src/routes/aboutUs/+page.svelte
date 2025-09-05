<script lang="ts">
	import EmmaPic from "$lib/assets/Emma.jpg?inline";
	import EnamPic from "$lib/assets/Enam.jpg?inline";
	import JosephPic from "$lib/assets/Joseph.jpg?inline";
	import BgPic from "$lib/assets/Background.jpg?inline";
</script>

<main>
	<div class="shadow-lg overflow-hidden">
		<!-- Background image and blur overlay as background layer -->
		<div
			class="absolute inset-0 z-0"
			style={`background-image: url('${BgPic}'); background-size: cover; background-position: center; background-repeat: no-repeat;`}
		>
			<div class="absolute inset-0 bg-base-100/80"></div>
		</div>

		<!-- All content sits above the overlay -->
		<div class="relative z-10 mt-16 mb-10">
			<div class="justify-center">
				<h1 class=" text-6xl text-center text-warning">
					We are a team of passionate individuals with diverse skills,
					working together to push boundaries to <em>cause</em> an
					impact. <br />
				</h1>
				<div>
					<p
						class="text-base-content text-lg-8xl pt-6 text-center tracking-widest font-light"
					>
						Our mission is to help people take control of their
						money with clarity and confidence. <br />
						By making it simple to track wallets, record transactions,
						and monitor balances, we empower users to stay organized
						and make smarter financial decisions.
					</p>
				</div>
			</div>

			<!-- Images and names of people -->
			<div>
				<p
					class="text-base-content text-2xl text-center font-sans font-semibold mt-6 mb-4"
				>
					Meet Our Team Members.
				</p>
				<div
					class="grid md:grid-cols-1 md:grid-rows-8 lg:grid-cols-2 lg:grid-rows-4 xl:grid-rows-4 xl:grid-cols-2 2xl:grid-rows-3 2xl:grid-cols-3 gap-4 pt-7 ml-16 justify-center"
				>
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src={EnamPic}
								alt="Team member"
								class="object-cover mt-7"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									KODJOH-KPAKPASSOU, ENAM A.-M.
								</h2>
								<p class="text-sm mt-1">
									Full-Stack Developer and Group Leader
								</p>
							</div>
						</figure>
					</div>

					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src={EmmaPic}
								alt="Team member"
								class="object-cover mt-4"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									ACQUAH, EMMANUEL A. K.
								</h2>
								<p class="text-sm mt-1">Frontend Developer</p>
							</div>
						</figure>
					</div>
					<!-- card 1 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src={JosephPic}
								alt="Team member"
								class="mt-4 object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									WIAFE, JOSEPH KWAKU
								</h2>
								<p class="text-sm mt-1">Frontend Developer</p>
							</div>
						</figure>
					</div>

					<!-- card 2 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src="https://randomuser.me/api/portraits/women/65.jpg"
								alt="Team member"
								class="min-h-full object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									NORMANYO, LESLIE DELA
								</h2>
								<p class="text-sm mt-1">Frontend Developer</p>
							</div>
						</figure>
					</div>

					<!-- card 3 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src="https://randomuser.me/api/portraits/men/74.jpg"
								alt="Team member"
								class="min-h-full object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									AHIAYIBOR, MAWUFEMOR
								</h2>
								<p class="text-sm mt-1">Backend Developer</p>
							</div>
						</figure>
					</div>

					<!-- card 4 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src="https://randomuser.me/api/portraits/women/24.jpg"
								alt="Team member"
								class="min-h-full object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									SARFO, MICHELLE N. A. ASANTEWAA
								</h2>
								<p class="text-sm mt-1">Frontend Developer</p>
							</div>
						</figure>
					</div>

					<!-- card 5 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src="https://randomuser.me/api/portraits/men/18.jpg"
								alt="Team member"
								class="min-h-full object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									WIAFE, RICHMOND KOFI
								</h2>
								<p class="text-sm mt-1 text-alert">
									REFUSED TO PARTICIPATE
								</p>
							</div>
						</figure>
					</div>

					<!-- card 6 -->
					<div
						class="card card-sm rounded-2xl bg-base-100 w-96 max-h-100 shadow-sm"
					>
						<figure class="relative rounded-2xl min-h-full">
							<img
								src="https://randomuser.me/api/portraits/women/56.jpg"
								alt="Team member"
								class="min-h-full object-cover"
							/>
							<div
								class="absolute bottom-0 w-90 bg-base-300/90 mb-2 rounded-2xl p-3 text-center"
							>
								<h2 class="font-semibold text-lg">
									AKPA, CLIFF ANUM
								</h2>
								<p class="text-sm mt-1">Full-Stack Developer</p>
							</div>
						</figure>
					</div>
				</div>
			</div>
		</div>
	</div>
</main>
