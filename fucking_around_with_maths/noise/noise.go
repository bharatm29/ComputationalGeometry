package main

import (
	"math/rand"

	"github.com/aquilax/go-perlin"
	rl "github.com/gen2brain/raylib-go/raylib"
)

const (
	WIDTH  int32 = 600
	HEIGHT int32 = 600
)

func main() {
	rl.InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window")
	defer rl.CloseWindow()

	rl.SetConfigFlags(rl.FlagMsaa4xHint) // Enables Anti-aliasing if available
	rl.SetTargetFPS(60)

	noise := perlin.NewPerlinRandSource(2, 2, 4, rand.NewSource(rand.Int63()))

	inc := 0.01
    start := 0.0

	for !rl.WindowShouldClose() {

		rl.BeginDrawing()

		{
			rl.ClearBackground(rl.Black)

			yoff := start
			for y := int32(0); y < HEIGHT; y++ {
				xoff := start
				for x := int32(0); x < WIDTH; x++ {
					mappedNoise := ((noise.Noise2D(xoff, yoff) + 1) / 2) // converting [-1, 1] to [0, 1]
					r := uint8(mappedNoise * 255)
					rl.DrawPixel(x, y, rl.Color{R: r, B: r, G: r, A: 255})
                    xoff += inc
				}
                yoff += inc
			}

            start += 0.01
		}

		rl.EndDrawing()
	}
}
