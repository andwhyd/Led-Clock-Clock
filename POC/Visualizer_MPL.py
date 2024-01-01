import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Annulus, Polygon, Circle
from matplotlib.animation import FuncAnimation
from digits import digits


class Clock:
    def __init__(self, ax, center=(0, 0), spoke_num=24, key="r"):
        self.ax = ax
        self.center = center
        self.spoke_num = spoke_num
        self.hands = [3, 3, 3]
        self.goal_hands = self.hands.copy()
        self.complete = True
        self.key = key
        self.patches = []

        self.spoke_colors = ["#464845", "white"]  # 0, 1
        self.base_color = "#4c4e4b"
        self.circle_color = "#6b6e69"

        self.draw_base()
        self.draw_spokes()
        self.draw_circles()

    def draw_base(self):
        base_circle = Circle(
            self.center, radius=150 / 2, edgecolor="none", facecolor=self.base_color
        )
        self.ax.add_patch(base_circle)

    def draw_spokes(self):
        angles = np.linspace(0, 2 * np.pi, self.spoke_num, endpoint=False)
        base_spoke = np.array(
            [
                [10, 0],
                [15, 1.5],
                [23, 2.5],
                [75, 2.5],
                [75, -2.5],
                [23, -2.5],
                [15, -1.5],
            ]
        )

        for i in range(self.spoke_num):
            angle = angles[i]
            rotation_matrix = np.array(
                [[np.cos(angle), -np.sin(angle)], [np.sin(angle), np.cos(angle)]]
            )
            rotated_spoke = np.dot(base_spoke, rotation_matrix.T) + np.array(
                self.center
            )
            spoke = Polygon(
                rotated_spoke,
                closed=True,
                edgecolor="none",
                facecolor=self.spoke_colors[i in self.hands],
            )
            self.ax.add_patch(spoke)
            self.patches.append(spoke)

    def draw_circles(self):
        top_circle = Circle(
            self.center, radius=25 / 2, edgecolor="none", facecolor=self.circle_color
        )
        ring_circle = Annulus(
            self.center,
            r=155 / 2,
            width=10,
            edgecolor="none",
            facecolor=self.circle_color,
        )
        self.ax.add_patch(top_circle)
        self.ax.add_patch(ring_circle)

    def set_hands(self, hands):
        self.complete = False
        for j, hand in enumerate(hands):
            if hand < 0:
                self.goal_hands[j] = self.hands[j]
            else:
                self.goal_hands[j] = hand

    def update(self):
        self.complete = self.goal_hands == self.hands
        for j, (hand, goal) in enumerate(zip(self.hands, self.goal_hands)):
            if hand != goal:
                cw = (goal - hand) % 24
                ccw = (hand - goal) % 24
                self.patches[hand].set_facecolor(self.spoke_colors[0])
                if cw <= ccw:
                    self.patches[(hand + 1) % 24].set_facecolor(
                        self.spoke_colors[1]
                    )
                    self.hands[j] = (hand + 1) % 24
                else:
                    self.patches[(hand - 1) % 24].set_facecolor(
                        self.spoke_colors[1]
                    )
                    self.hands[j] = (hand - 1) % 24
            else:
                self.patches[hand].set_facecolor(self.spoke_colors[1])


class ClockDigit:
    def __init__(self, ax, coords, value=0):
        self.value = value
        self.complete = True
        self.clocks = []

        for coord in coords:
            clock = Clock(ax, center=coord)
            self.clocks.append(clock)

    def update(self):
        self.complete = True
        for clock in self.clocks:
            clock.update()
            if clock.complete == False:
                self.complete = False

    def set_value(self, value):
        for j, hands in enumerate(digits[value]):
            self.clocks[j].set_hands(hands)

    def reset_digit(self):
        for clock in self.clocks:
            clock.set_hands([3, 3, 3])


class ClockPanel:
    def __init__(
        self,
        num_digits=4,
        spacing=200,
        digit_size=(3, 2),
        digit_spacing=175,
    ):
        self.fig, self.ax = plt.subplots()
        self.fig.set_facecolor("ivory")
        self.num_digits = num_digits
        self.digit_spacing = digit_spacing
        self.digit_size = digit_size

        self.paused = True
        self.complete = True
        self.digits = []
        self.all_coords = []

        for j in range(num_digits):
            start = j * ((digit_size[1] - 1) * digit_spacing + spacing)
            x_coords = (np.arange(digit_size[1]) * digit_spacing) + start
            y_coords = np.arange(digit_size[0]) * digit_spacing
            x, y = np.meshgrid(x_coords, y_coords)
            coords = np.column_stack([x.ravel(), y.ravel()])
            digit = ClockDigit(self.ax, coords)
            self.all_coords.append(coords)
            self.digits.append(digit)
        self.all_coords = np.concatenate(self.all_coords)

        self.fig.canvas.mpl_disconnect(self.fig.canvas.manager.key_press_handler_id)
        self.fig.canvas.mpl_connect("key_press_event", self.on_key)
        self.fig.canvas.mpl_connect("button_press_event", self.on_mouse)

    def on_key(self, event):
        if event.key == "u":
            if self.paused:
                self.animation.resume()
            else:
                self.animation.pause()
            self.paused = not self.paused
        elif event.key == "r":
            for digit in self.digits:
                digit.reset_digit()
            self.update()
        elif event.key == "t":
            for digit in self.digits:
                digit.set_value(np.random.randint(0, 10))
            self.update()
        if event.key in ("escape", "q"):
            plt.close()

    def on_mouse(self, event):
        if event.inaxes != self.ax:
            return
        distances = np.linalg.norm(self.all_coords - (event.xdata, event.ydata), axis=1)
        within_radius = distances < (self.digit_spacing - 25) / 2
        ind = np.where(within_radius)[0]
        if len(ind) > 0:
            clock = self.digits[
                ind[0] // (self.digit_size[0] * self.digit_size[1])
            ].clocks[ind[0] % (self.digit_size[0] * self.digit_size[1])]
            hands = clock.hands.copy()
            if event.button == 1:
                hands[0] = np.random.randint(0, 24)
                clock.set_hands(hands)
            elif event.button == 3:
                hands[1] = np.random.randint(0, 24)
                clock.set_hands(hands)
            elif event.button == 2:
                print(clock.goal_hands)
                print(clock.hands)
            self.update()

    def update(self, frame=0):
        self.complete = True
        for digit in self.digits:
            digit.update()
            if digit.complete == False:
                self.complete = False
        self.fig.canvas.draw()
        if self.complete:
            self.animation.pause()
        else:
            self.animation.resume()
        self.paused = self.complete

    def show(self):
        self.ax.axis("equal")
        self.ax.axis("off")
        self.animation = FuncAnimation(
            self.fig, self.update, interval=10, cache_frame_data=False
        )
        self.animation.pause()
        self.paused = True
        plt.show()


if __name__ == "__main__":
    clock_panel = ClockPanel()
    clock_panel.show()
