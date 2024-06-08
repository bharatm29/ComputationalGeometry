package main

import (
	rl "github.com/gen2brain/raylib-go/raylib"
)

const (
	WIDTH  = 900
	HEIGHT = 900
)

func calculateQuadraticBezier(p0, controlp, p2 rl.Vector2, t float32) rl.Vector2 {
	v0 := rl.Vector2Lerp(p0, controlp, t)
	v1 := rl.Vector2Lerp(controlp, p2, t)

	rl.DrawLineV(v0, v1, rl.Maroon)

	point := rl.NewVector2(rl.Lerp(v0.X, v1.X, t), rl.Lerp(v0.Y, v1.Y, t))

	return point
}

func main() {
	rl.SetConfigFlags(rl.FlagMsaa4xHint)

	rl.InitWindow(WIDTH, HEIGHT, "Fking Bored")

	defer rl.CloseWindow()

	rl.SetTargetFPS(60)

	p0 := rl.NewVector2(0, HEIGHT/2)
	p1 := rl.NewVector2(WIDTH, HEIGHT/2)
	controlp2 := rl.Vector2{X: 400, Y: 300}

	points := []rl.Vector2{p0}

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()

		{
			rl.ClearBackground(rl.Black)

			controlp1 := rl.Vector2{X: float32(rl.GetMouseX()), Y: float32(rl.GetMouseY())}

			if rl.IsKeyDown(rl.KeySpace) {
				rl.DrawCircleV(p0, 8, rl.Red)
				rl.DrawCircleV(p1, 8, rl.Red)
			}

			rl.DrawCircleV(controlp1, 8, rl.Red)

			rl.SetLineWidth(2)
			for t := float32(0.0); t <= 1.00001; t += 0.05 {
				lerp1 := calculateQuadraticBezier(p0, controlp1, controlp2, t)
				lerp2 := calculateQuadraticBezier(controlp1, controlp2, p1, t)

				rl.DrawLineV(lerp1, lerp2, rl.Red)

				point := rl.Vector2Lerp(lerp1, lerp2, t)
				points = append(points, point)
			}

			if rl.IsKeyDown(rl.KeySpace) {
				rl.DrawLineStrip(points, rl.Blue)
			}

			points = []rl.Vector2{}
		}

		rl.EndDrawing()
	}
}
