import adapter from '@sveltejs/adapter-static';
import {vitePreprocess} from '@sveltejs/vite-plugin-svelte';

/** @type {import('@sveltejs/kit').Config} */
const config = {
  preprocess: vitePreprocess(),
  kit: {
    version: {pollInterval: 0},
    output: {bundleStrategy: 'inline'},
    router: {type: 'hash'},
    adapter: adapter({fallback: 'fallback.html'})
  }
};
export default config;