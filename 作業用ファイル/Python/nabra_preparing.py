import numpy as np
import sympy as sm

from sympy import Abs, I, AccumBounds, symbols
from sympy import symbols, sin
from sympy import symbols, cos
sm.init_printing(use_unicode=False, wrap_line=True)


def limit_sup(func, var, start=1, end=1000):
    values = [float(func.subs(var, i).evalf()) for i in range(start, end + 1)]
    one_value = np.round(values)
    return max(one_value)

def limit_inf(func, var, start=1, end=1000):
    values = [float(func.subs(var, i).evalf()) for i in range(start, end + 1)]
    one_value = np.round(values)
    return min(one_value)

# 確認
n = sm.Symbol('n', integer=True)
func = sm.sin(n)
print("limsup:", limit_sup(func, n))
print("liminf:", limit_inf(func, n))




#処理一覧
#微分　-> sm.diff(f)
#積分　-> sm.integrate(f,x)
#極限　-> sm.limit(f,x,何に近づけるか)
#上極限　-> limit_sup(自作関数)
#下極限　-> limit_inf(自作関数)
#
#
#
#
#
#
#
#
#
#
#
#
#