package main

import (
	"fmt"
	"math"
	"math/rand"

	"github.com/aquilax/go-perlin"
	rl "github.com/gen2brain/raylib-go/raylib"
)

const (
	WIDTH  int32 = 800
	HEIGHT int32 = 800
)

func main() {
	rl.InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window")
	defer rl.CloseWindow()

	rl.SetConfigFlags(rl.FlagMsaa4xHint) // Enables Anti-aliasing if available
	rl.SetTargetFPS(60)

	noise := perlin.NewPerlinRandSource(2, 2, 4, rand.NewSource(rand.Int63()))

	inc := 0.01
	start := 0.0

	scale := int32(10)

	cols := WIDTH / scale
	rows := HEIGHT / scale

	for !rl.WindowShouldClose() {

		rl.BeginDrawing()

		{
			rl.ClearBackground(rl.White)

			yoff := start
			for y := int32(0); y < rows; y++ {
				xoff := start
				for x := int32(0); x < cols; x++ {
					mappedNoise := ((noise.Noise2D(xoff, yoff) + 1) / 2)
					r := uint8(mappedNoise * 255)
					xoff += inc

					_ = rl.Color{R: r, G: r, B: r, A: r}

					v := vectorFromAngle(rl.Pi * mappedNoise) // Create a vector from the angle
					v = rl.Vector2Scale(v, float32(scale))

					// Draw the line
					rl.DrawLineV(rl.Vector2{X: float32(x * scale), Y: float32(y * scale)},
						rl.Vector2{X: float32(x*scale) + v.X, Y: float32(y*scale) + v.Y},
						rl.Black)

					// rl.DrawRectangle(x*scale, y*scale, int32(scale), int32(scale), clr)
				}
				yoff += inc
			}

			rl.DrawText(fmt.Sprintf("%d", rl.GetFPS()), 20, 20, 10, rl.DarkBlue)
			// start += 0.01
		}

		rl.EndDrawing()
	}
}

func vectorFromAngle(angle float64) rl.Vector2 {
	return rl.Vector2{X: float32(math.Cos(angle)), Y: float32(math.Sin(angle))}
}
