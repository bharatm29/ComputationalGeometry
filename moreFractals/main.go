package main

import (
	"fmt"
	"math"
	"slices"

	rl "github.com/gen2brain/raylib-go/raylib"
)

const (
	k          int  = -8
	circles    int  = 11
	resolution int  = 100
	inside     int  = 1
	showCircle bool = true
)

type Orbit struct {
	parent *Orbit
	child  *Orbit

	angle float64

	x float32
	y float32
	r float32

	speed float32
	level int
}

func (O *Orbit) Show() {
	rl.DrawCircleLines(int32(O.x), int32(O.y), O.r, rl.Beige)
}

func (O *Orbit) AddChild() *Orbit {
	newR := O.r / 3.0
	newX := O.x + O.r + newR
	newY := O.y

	child := &Orbit{
		x: newX,
		y: newY,
		r: newR,

		parent: O,
		child:  nil,

		angle: -rl.Pi / 2,
		level: O.level + 1,
		speed: float32(math.Pow(float64(k), float64(O.level))) * (math.Pi / 180) / float32(resolution),
	}

	O.child = child

	return child
}

func (O *Orbit) Update() {
	if O.parent == nil {
		return
	}

	O.angle += float64(O.speed)
	rSum := O.parent.r + (O.r * float32(inside))

	O.x = O.parent.x + rSum*float32(math.Cos(O.angle))
	O.y = O.parent.y + rSum*float32(math.Sin(O.angle))
}

func main() {
	const WIDTH int32 = 600
	const HEIGHT int32 = 600

	rl.InitWindow(WIDTH, HEIGHT, "raylib [core] example - basic window")
	defer rl.CloseWindow()

	rl.SetConfigFlags(rl.FlagMsaa4xHint) // Enables Anti-aliasing if available
	rl.SetTargetFPS(60)

	core := Orbit{x: float32(WIDTH) / 2, y: float32(HEIGHT / 2), r: 150, parent: nil, angle: 0, speed: 0.01, child: nil, level: 0}

	orbit := &core

	for i := 0; i < circles; i++ {
		orbit = orbit.AddChild()
	}

	lastChild := orbit
	path := []rl.Vector2{}

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()

		{
			rl.ClearBackground(rl.DarkGray)

			for i := 0; i < resolution; i++ {

				orbitalOrbits := &core

				for orbitalOrbits != nil {
					orbitalOrbits.Update()

					if showCircle {
						orbitalOrbits.Show()
					}

					orbitalOrbits = orbitalOrbits.child
				}
				pos := rl.Vector2{X: float32(lastChild.x), Y: float32(lastChild.y)}

				if !slices.Contains(path, pos) {
					path = append(path, pos)
				}

			}

			rl.DrawLineStrip(path, rl.Orange)
		}

		rl.DrawText(fmt.Sprintf("%d", rl.GetFPS()), 20, 20, 20, rl.Blue)

		rl.EndDrawing()
	}
}

/*


	angle := 0.0

*
    var r1 float32 = 100
    var x1 int32 = 300
    var y1 int32 = 300

    rl.DrawCircleLines(x1, y1, r1, rl.Beige)

    r2 := r1 * 0.5

    rsum := float64(r1 + r2)

    x2 := x1 + int32(rsum*math.Cos(angle))
    y2 := y1 + int32(rsum*math.Sin(angle))

    rl.DrawCircleLines(x2, y2, r2, rl.Beige)
*/
