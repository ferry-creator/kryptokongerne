<script context="module">
  export const prerender = true

  export const load = async ({ fetch }) => {
    return {
      props: {
        recentPosts: await fetch('/posts.json?limit=2').then((res) => res.json())
      }
    }
  }
</script>

<script>
  import ButtonLink from '$lib/components/ButtonLink.svelte'
  import PostPreview from '$lib/components/PostPreview.svelte'
  import { name } from '$lib/info.js'

  import { fly } from 'svelte/transition'
  import typewriter from '$lib/transitions/typewriter'
  import { onMount } from 'svelte';

  let mounted = false
  let mountedDelay = false
  onMount(() => {
    mounted = true
    setTimeout(() => {
      mountedDelay = true
    }, 200)
  })

  export let recentPosts
</script>

<svelte:head>
  <title>{name}</title>
</svelte:head>

<div class="flex flex-col flex-grow">
  <!-- replace with a bio about you, or something -->
  <div class="flex items-center justify-center text-xl h-40">
    {#if mounted}
      <h1 class="text-center">
        <span class="text-6xl text-transparent bg-clip-text GRADIENT_DANCE">
          <span class="text-4xl" in:typewriter={{speed: 1.5, delay: 300}}>
            ░　Welcome to the domain of　░
          </span>
          <br>
          <span class="font-extralight" in:typewriter={{speed: 1.5, delay: 2200}}>
            krypto ♢ kongerne
          </span>
        </span>
      </h1>
    {/if}
  </div>
  <div class="flex items-center justify-center text-xl h-36">
    {#if mountedDelay}
      <div in:fly="{{ y: 20, duration: 1000 }}">
        <ButtonLink size="large" blank href="https://github.com/ferry-creator/kryptokongerne">
          <slot slot="icon-start">
            <svg
              class="fill-black dark:fill-white h-6 w-6"
              xmlns="http://www.w3.org/2000/svg"
              viewBox="0 0 24 24"
            >
              <path
                d="M10.9,2.1c-4.6,0.5-8.3,4.2-8.8,8.7c-0.5,4.7,2.2,8.9,6.3,10.5C8.7,21.4,9,21.2,9,20.8v-1.6c0,0-0.4,0.1-0.9,0.1 c-1.4,0-2-1.2-2.1-1.9c-0.1-0.4-0.3-0.7-0.6-1C5.1,16.3,5,16.3,5,16.2C5,16,5.3,16,5.4,16c0.6,0,1.1,0.7,1.3,1c0.5,0.8,1.1,1,1.4,1 c0.4,0,0.7-0.1,0.9-0.2c0.1-0.7,0.4-1.4,1-1.8c-2.3-0.5-4-1.8-4-4c0-1.1,0.5-2.2,1.2-3C7.1,8.8,7,8.3,7,7.6C7,7.2,7,6.6,7.3,6 c0,0,1.4,0,2.8,1.3C10.6,7.1,11.3,7,12,7s1.4,0.1,2,0.3C15.3,6,16.8,6,16.8,6C17,6.6,17,7.2,17,7.6c0,0.8-0.1,1.2-0.2,1.4 c0.7,0.8,1.2,1.8,1.2,3c0,2.2-1.7,3.5-4,4c0.6,0.5,1,1.4,1,2.3v2.6c0,0.3,0.3,0.6,0.7,0.5c3.7-1.5,6.3-5.1,6.3-9.3 C22,6.1,16.9,1.4,10.9,2.1z"
              /></svg
            >
          </slot>
          View on GitHub
          <slot slot="icon-end" />
        </ButtonLink>
      </div>
    {/if}
  </div>

  <!-- recent posts -->
  <h2 class="flex items-baseline gap-4 !mb-2">
    Recent Posts
    <ButtonLink href="/posts" size="small" raised={false} class="opacity-60">View All</ButtonLink>
  </h2>
  <div class="grid gap-4 grid-cols-1 sm:grid-cols-2">
    {#each recentPosts as post}
      <div class="flex p-4 border border-slate-300 dark:border-slate-700 rounded-lg">
        <PostPreview {post} small />
      </div>
    {/each}
  </div>
</div>

<style lang="postcss">
  .GRADIENT_DANCE {
    background-image: linear-gradient(60deg,
    #0048ff,#0048ff,#ff0062,#ff0062,#f200ff,#6b00ff,#6b00ff,#6b00ff,#00ffbd,#d9ff00,#00ffb3,
    #f200ff,#6b00ff,#6b00ff,#6b00ff,#0048ff
    );
    background-size: 300%;

    -webkit-animation: dance 2.8s linear infinite;
    -moz-animation: dance 2.8s linear infinite;
    animation: dance 2.8s linear infinite;
  }

  @-webkit-keyframes dance {
    0%{background-position:0% 50%}
    100%{background-position:300% 50%}
    /* 100%{background-position:0% 50%} */
  }
  @-moz-keyframes dance {
    0%{background-position:0% 50%}
    100%{background-position:300% 50%}
    /* 100%{background-position:0% 50%} */
  }

  @keyframes dance {
    0%{background-position:0% 50%}
    100%{background-position:300% 50%}
    /* 100%{background-position:0% 50%} */
  }
</style>
