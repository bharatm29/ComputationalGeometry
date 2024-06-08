package main

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
)

/*
* ********************************
* * The Sierpinski Triangle
*   WIDTH        = 1920
*   HEIGHT       = 1080
*   LINE_LEN     = 4
*   ROTATE_ANGLE = rl.Pi / 3
*   ITERATIONS   = 9
*   START_SEQUENCE = "F"
*
*   'F': "G-F-G",
*   'G': "F+G+F",
* ********************************
* * The Heighway Dragon Curve
*   WIDTH        = 1920
*   HEIGHT       = 1080
*   LINE_LEN     = 10
*   ROTATE_ANGLE = rl.Pi / 2
*   ITERATIONS   = 7
*   START_SEQUENCE = "F"
*
*   'F': "F-G",
*   'G': "F+G",
* ********************************
 */

const (
	WIDTH        = 1920
	HEIGHT       = 1080
	LINE_LEN     = 4
	ROTATE_ANGLE = rl.Pi / 3
	ITERATIONS   = 9

	START_SEQUENCE = "F"
)

type Drawer struct {
	rule map[rune]string

	input     string
	nextState string

	pos int

	coords rl.Vector2

	angle     float64
	angleSign float64
}

func NewDrawer(input string) *Drawer {
	d := &Drawer{
		input:     input,
		nextState: "",
		pos:       0,
		angle:     0.0,
		angleSign: 1,
	}

	d.rule = map[rune]string{
		'F': "G-F-G",
		'G': "F+G+F",
	}

	d.coords = rl.NewVector2(float32(WIDTH)*0.23, HEIGHT-50)

	return d
}

func (d *Drawer) nextDraw() (stop bool) {
	if d.pos >= len(d.input) {
		return true
	}

	switch d.input[d.pos] {
	case 'F', 'G':
		d.nextState += d.rule[rune(d.input[d.pos])]
		nextCoords := rl.NewVector2(
			d.coords.X+float32(float64(LINE_LEN)*math.Cos(d.angle*d.angleSign)),
			d.coords.Y+float32(float64(LINE_LEN)*math.Sin(d.angle*d.angleSign)),
		)

		rl.DrawLine(int32(d.coords.X), int32(d.coords.Y), int32(nextCoords.X), int32(nextCoords.Y), rl.Green)
		d.coords = nextCoords
	case '+':
		d.angle -= ROTATE_ANGLE
		d.nextState += "+"
	case '-':
		d.angle += ROTATE_ANGLE
		d.nextState += "-"
	default:
		panic("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH")
	}

	d.pos++

	return false
}

func (d *Drawer) switchState() {
	d.input = d.nextState
	d.nextState = ""
	d.pos = 0
	d.angle = 0
	d.angleSign *= -1
	d.coords = rl.NewVector2(float32(WIDTH)*0.23, HEIGHT-50)
}

func main() {
	rl.InitWindow(WIDTH, HEIGHT, "L-Systems")

	defer rl.CloseWindow()

	rl.SetConfigFlags(rl.FlagMsaa4xHint)
	rl.SetTargetFPS(60)

	for !rl.WindowShouldClose() {
		rl.BeginDrawing()
		{
			rl.ClearBackground(rl.DarkBlue)
			d := NewDrawer(START_SEQUENCE)
			for i := 0; i < ITERATIONS; i++ {
				for !d.nextDraw() {
				}

				d.switchState()
				rl.ClearBackground(rl.DarkBlue)
			}
		}
		rl.EndDrawing()
	}
}
