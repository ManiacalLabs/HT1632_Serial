from bibliopixel.animation import BaseMatrixAnim
import bibliopixel.colors as colors
from bibliopixel import font


class Test(BaseMatrixAnim):

    def __init__(self, layout):
        super().__init__(layout)

    def step(self, amt=1):
        self.layout.all_off()
        self.layout.drawLine(self._step, 0, self._step, self.height - 3, colors.Red)

        self._step += 1
        if(self._step >= self.width):
            self._step = 0
