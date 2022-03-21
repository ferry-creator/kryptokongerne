<script>
  import '../app.css'
  import '../prism.css'
  import 'focus-visible'
  import MoonIcon from 'heroicons-svelte/solid/MoonIcon.svelte'
  import SunIcon from 'heroicons-svelte/solid/SunIcon.svelte'

  import RandomFloaties from '$lib/components/RandomFloaties.svelte'

  import { browser } from '$app/env'
  import { name } from '$lib/info'
  import { fly } from 'svelte/transition'
  import { onMount } from 'svelte';
  

  let mounted = false
  onMount(() => {
    mounted = true
  })

  let prefersLight = browser ? Boolean(JSON.parse(localStorage.getItem('prefersLight'))) : false
</script>

<!-- SILLY GIFS!! -->
<RandomFloaties />

<div class="flex flex-col min-h-screen">
  <div class="mx-auto flex flex-col flex-grow w-full max-w-4xl">
    <div class="flex h-24 px-4 py-2 justify-between items-center">
      <a href="/" class="h-full pr-4">
        {#if mounted}
          <div class="h-full" in:fly="{{ y: 10, duration: 500 }}">
            {#if prefersLight}
            <img src="/logo.svg" alt="logo" class="h-full" />
            {:else}
              <img src="/logo-dark.svg" alt="logo" class="h-full" />
            {/if}
          </div>
        {/if}
      </a>
      <!-- <h2
        class="!text-transparent bg-clip-text bg-gradient-to-r from-blue-500 to-teal-500 dark:from-violet-500 dark:to-pink-500"
      >
        <a class="text-lg sm:text-2xl font-bold" href="/">
          {name}
        </a>
      </h2> -->
      {#if browser}
        <button
          type="button"
          role="switch"
          aria-label="Toggle Dark Mode"
          aria-checked={!prefersLight}
          class="h-4 w-4 sm:h-8 sm:w-8 sm:p-1"
          on:click={() => {
            prefersLight = !prefersLight
            localStorage.setItem('prefersLight', prefersLight.toString())

            if (prefersLight) {
              document.querySelector('html').classList.remove('dark')
            } else {
              document.querySelector('html').classList.add('dark')
            }
          }}
        >
          {#if mounted}
            <div in:fly="{{ y: 10, duration: 500 }}">
              {#if prefersLight}
                <MoonIcon class="text-slate-500" />
              {:else}
                <SunIcon class="text-slate-400" />
              {/if}
            </div>
          {/if}
        </button>
      {/if}
    </div>
    <main
      class="prose prose-slate prose-sm sm:prose sm:prose-slate sm:prose-lg sm:max-w-none dark:prose-invert flex flex-col w-full flex-grow py-4 px-4"
    >
      <slot />
    </main>
  </div>
</div>
