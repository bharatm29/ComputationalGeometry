package main

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
)

const (
	WIDTH  = 800
	HEIGHT = 800

	DIVERSION = (2 * rl.Pi) / 11
	ratio     = 0.75
)

func branch(posX, posY, len int32, angle float64) {
	endX := posX + int32(float64(len)*math.Cos(angle))
	endY := posY - int32(float64(len)*math.Sin(angle))

	// drawing the actual branch
	rl.DrawLine(posX, posY, endX, endY, rl.Beige)

	if len > 10 {
		// right branch
		branch(endX, endY, int32(float64(len)*ratio), angle+DIVERSION)

		// right branch
		branch(endX, endY, int32(float64(len)*ratio), angle-DIVERSION)
	}
}

func main() {
	rl.InitWindow(WIDTH, HEIGHT, "Fractal Tree")
	defer rl.CloseWindow()

	rl.SetConfigFlags(rl.FlagMsaa4xHint) // Enables Anti-aliasing if available
	rl.SetTargetFPS(60)

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
		{
			rl.ClearBackground(rl.Color{R: 51, G: 51, B: 51, A: 255})
			var len int32 = 150
			rl.DrawLine(WIDTH/2, HEIGHT, WIDTH/2, HEIGHT-len, rl.Beige)
			branch(WIDTH/2, HEIGHT-len, len, rl.Pi/2)
		}
		rl.EndDrawing()
	}
}
