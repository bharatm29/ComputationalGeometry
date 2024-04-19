package main

import (
	"fmt"
	"math/rand"
	"slices"

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

	xoff := 0.0
	xoff1 := 10000.0
	noise := perlin.NewPerlinRandSource(2, 2, 3, rand.NewSource(rand.Int63()))

    start := 0.0

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()

		{
			rl.ClearBackground(rl.Black)

			mappedNoise1 := ((noise.Noise1D(xoff) + 1) / 2) // converting [-1, 1] to [0, 1]
			x := int32(mappedNoise1 * float64(WIDTH))

			mappedNoise2 := ((noise.Noise1D(xoff1) + 1) / 2)
			y := int32(mappedNoise2 * float64(HEIGHT))

			if x > WIDTH {
				x = WIDTH
			}

			if x < 0 {
				x = 0
			}

			xoff += 0.01
			xoff1 += 0.02

			rl.DrawText(fmt.Sprintf("(%d, %d)", x, y), 20, 20, 20, rl.Red)
            rl.DrawLine(0, HEIGHT / 2, WIDTH, HEIGHT / 2, rl.DarkBlue)

			var i int32

			pathOff := start
            path := []rl.Vector2{}

			for i = 0; i < WIDTH; i++ {
				mappedNoise2 := ((noise.Noise1D(pathOff) + 1) / 2)
				y := int32(mappedNoise2 * float64(HEIGHT))

				pos := rl.Vector2{X: float32(i), Y: float32(y)}

				if !slices.Contains(path, pos) {
					path = append(path, pos)
				}

				pathOff += 0.01

			}

            start += 0.01

			// rl.DrawCircle(x, y, 20, rl.Beige)

			rl.DrawLineStrip(path, rl.Beige)
		}

		rl.EndDrawing()
	}
}
