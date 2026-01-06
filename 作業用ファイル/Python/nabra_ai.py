import sympy as sm
import pygame
import numpy as np

class TextButton:
    def __init__(self, text, expr, position, color=(0, 0, 0), font_size=32):
        self.text = text
        self.expr = expr
        self.position = position
        self.color = color
        self.font_size = font_size
        self.font = pygame.font.Font(None, self.font_size)
        self.surface = self.font.render(self.text, True, self.color)
        self.rect = self.surface.get_rect(topleft=self.position)

    def draw(self, surface):
        surface.blit(self.surface, self.position)

    def is_clicked(self, pos):
        return self.rect.collidepoint(pos)

    def evaluate(self, value):
        x = sm.Symbol('x')
        return self.expr.subs(x, value).evalf()



def limit_sup(func, var, start=1, end=1000):
    values = [float(func.subs(var, i).evalf()) for i in range(start, end + 1)]
    one_value = np.round(values)
    return max(one_value)

pygame.init()
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("limsup シミュレーション")

x = sm.Symbol('x')

# 状態変数
selected_op = None
selected_func = None
result_text = None

# ボタン作成
limsup_btn = TextButton("limsup(x→∞)", None, (50, 100), color=(200, 0, 0))
sin_btn = TextButton("sin(x)", sm.sin(x), (50, 180), color=(0, 100, 255))
cos_btn = TextButton("cos(x)", sm.cos(x), (50, 230), color=(0, 100, 255))

# 関数の辞書（SymPy式と対応）
functions = {
    "sin(x)": sm.sin(x),
    "cos(x)": sm.cos(x)
}

clock = pygame.time.Clock()
running = True

while running:
    screen.fill((255, 255, 255))

    # ボタン描画
    limsup_btn.draw(screen)
    sin_btn.draw(screen)
    cos_btn.draw(screen)

    # 結果があれば表示
    if result_text:
        font = pygame.font.Font(None, 36)
        result_surface = font.render(f"結果: {result_text}", True, (0, 0, 0))
        screen.blit(result_surface, (300, 300))

    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()

            if limsup_btn.is_clicked(pos):
                selected_op = "limsup"

            elif sin_btn.is_clicked(pos):
                selected_func = "sin(x)"

            elif cos_btn.is_clicked(pos):
                selected_func = "cos(x)"

            # 条件がそろったら計算を実行
            if selected_op == "limsup" and selected_func:
                expr = functions[selected_func]
                result = limit_sup(expr, x)
                result_text = str(result)
                # 一度実行したらリセット
                selected_op = None
                selected_func = None

    clock.tick(60)

pygame.quit()
