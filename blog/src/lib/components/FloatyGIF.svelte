<script>
  export let src
  export let size
  export let top = 'auto', left = 'auto', right = 'auto'
  export let opacity

  // let y
</script>

<!-- <svelte:window bind:scrollY={y} /> -->
<!-- top: ${size > 100 ? `calc(${top} + ${y / 10}px)` : top}; -->

<img
  {src} alt="GIF" width={size}
  style={`
    left: ${left};
    right: ${right};
    top: ${top};
    opacity: ${opacity};
  `}
>

<style>
  img {
    position: absolute;
    z-index: -1;
  }
</style>
