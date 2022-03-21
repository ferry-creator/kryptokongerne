<script>
  import { onMount } from 'svelte'

  import FloatyGif from "./FloatyGIF.svelte"

  const randomPercent = () => `${Math.round(Math.random()*100)}%`
  const randomInArr = (arr) => arr[Math.floor(Math.random() * arr.length)]
  const randomBetween = (min, max) => {
    return Math.floor(Math.random() * (max - min + 1) + min)
  }

  const getURL = (file) => `/GIFS/optimized2/${file}`

  let w,h

  const blobPool = [
    "blob1.png",
    "blob2.png",
    "blob3.png",
    "blob4.png",
    "blob5.png",
    "blob6.png",
    "blob7.png",
    "blob8.png",
    "crown.png",
    "heart.png"
  ]

  const coinPool = [
    "coin.png",
    "bitcoin.png",
    "bitcoin2.png",
    "bitcoin2.png",
    "bitcoin2.png",
  ]

  const weirdPool = [
    "butterfly.png",
    "crown2.png",
  ]

  const ddiv = 20

  const randomLefties = (density) => {
    const blob_cnt = Math.round((randomBetween(4, 5) * density) / ddiv)
    let blob_images = []
    for (let i = 0; i < blob_cnt; i++) {
      blob_images.push(randomInArr(blobPool))
    }

    const blobs = blob_images.map((img) => ({
      src: getURL(img),
      top: randomPercent(),
      left: randomPercent(),
      size: randomBetween(50, 120),
      opacity: `${randomBetween(70, 90)}%`,
      seeds: [Math.random(), Math.random()]
    }))

    const coint_cnt = Math.round((randomBetween(0, 1) * density) / ddiv)
    let coin_images = []
    for (let i = 0; i < coint_cnt; i++) {
      coin_images.push(randomInArr(coinPool))
    }

    const coins = coin_images.map((img) => ({
      src: getURL(img),
      top: randomPercent(),
      left: randomPercent(),
      size: randomBetween(90, 150),
      opacity: `100%`,
      seeds: [Math.random(), Math.random()]
    }))

    return [
      ...blobs,
      ...coins
    ]
  }

  const randomRighties = (density) => {
    const blob_cnt = Math.round((randomBetween(2, 3) * density) / ddiv)
    let blob_images = []
    for (let i = 0; i < blob_cnt; i++) {
      blob_images.push(randomInArr(blobPool))
    }

    const blobs = blob_images.map((img) => ({
      src: getURL(img),
      top: randomPercent(),
      right: randomPercent(),
      size: randomBetween(50, 120),
      opacity: `${randomBetween(70, 90)}%`,
      seeds: [Math.random(), Math.random()]
    }))

    let coin_images = []
    const makeCoin = !!!randomBetween(0,4)
    if(makeCoin) coin_images.push(coinPool[2])

    const coins = coin_images.map((img) => ({
      src: getURL(img),
      top: randomPercent(),
      right: randomPercent(),
      size: randomBetween(90, 150),
      opacity: `100%`,
      seeds: [Math.random(), Math.random()]
    }))

    const crown_cnt = randomBetween(0, 1)
    let crown_images = []
    for (let i = 0; i < crown_cnt; i++) {
      crown_images.push(randomInArr(weirdPool))
    }
    crown_images.push(weirdPool[1])

    const crowns = crown_images.map((img) => ({
      src: getURL(img),
      top: randomPercent(),
      right: randomPercent(),
      size: randomBetween(80, 110),
      opacity: `100%`,
      seeds: [Math.random(), Math.random()]
    }))

    return [
      ...blobs,
      ...coins,
      ...crowns
    ]
  }

  let LEFT = null
  let RIGHT = null
  onMount(() => {
    LEFT = randomLefties(Math.ceil(h/100))
    RIGHT = randomRighties(Math.ceil(h/200))
    const frozenLEFT = LEFT
    const frozenRIGHT = RIGHT

    const AMP = 40
    const SPEED = 12

    let frame;

    let cnt = 0
		function loop() {
      frame = requestAnimationFrame(loop)

      // cnt = (cnt + 0.001) % 6.283185
      cnt = (cnt + 0.001)

			LEFT = LEFT.map((gif,i) => {
        const frzen = frozenLEFT[i]

        const R_AMP = [AMP * frzen.seeds[1], AMP * frzen.seeds[0]]
        const R_SPEED = [SPEED * frzen.seeds[0], SPEED * frzen.seeds[1]]
        const x_offset = Math.sin((cnt+frzen.seeds[0])*R_SPEED[0])*R_AMP[0]
        const y_offset = Math.sin((cnt+frzen.seeds[1])*R_SPEED[1])*R_AMP[1]
          // + frzen.size > 100 ? Math.round(y * (frzen.size * 0.0012)) : 0

        console.log(Math.round(y * (frzen.size * 0.0012)))

        return {
          ...gif,
          top: `calc(${frzen.top} + ${y_offset}px)`,
          left: `calc(${frzen.left} + ${x_offset}px)`
        }
      })

      RIGHT = RIGHT.map((gif,i) => {
        const frzen = frozenRIGHT[i]

        const R_AMP = [AMP * frzen.seeds[1], AMP * frzen.seeds[0]]
        const R_SPEED = [SPEED * frzen.seeds[0], SPEED * frzen.seeds[1]]
        const x_offset = Math.sin((cnt+frzen.seeds[0])*R_SPEED[0])*R_AMP[0]
        const y_offset = Math.sin((cnt+frzen.seeds[1])*R_SPEED[1])*R_AMP[1]
          // + frzen.size > 100 ? Math.round(y * (frzen.size * 0.0012)) : 0

        return {
          ...gif,
          top: `calc(${frzen.top} + ${y_offset}px)`,
          right: `calc(${frzen.right} + ${x_offset}px)`
        }
      })
		}

		loop()

		return () => cancelAnimationFrame(frame)
	})

  let y
</script>

<svelte:window bind:scrollY={y} />

<div class="left" bind:clientWidth={w} bind:clientHeight={h}>
  {#if !!LEFT}
    {#each LEFT as G}
      <FloatyGif {...G} />
    {/each}
  {/if}
</div>

<div class="right">
  {#if !!RIGHT}
    {#each RIGHT as G}
      <FloatyGif {...G} />
    {/each}
  {/if}
</div>

<style>
  .left, .right {
    position: absolute;
    /* background: rgba(255, 255, 0, 0.214); */
  }

  .left {
    left: -30px;
    bottom: 150px;
    height: calc(100% - 150px);
    width: calc(40vw - 400px);
  }

  .right {
    top: 250px;
    bottom: 150px;
    height: calc(100% - 400px); /* top + bottom = 550px */
    width: calc(40vw - 420px);
    right: -30px;
  }

  @media only screen and (max-width: 1200px) {
    .left, .right {
      display: none;
    }
  }
</style>
