import random
import time
from reportlab.platypus import SimpleDocTemplate, Paragraph, Table, Spacer
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.pagesizes import A4
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.cidfonts import UnicodeCIDFont

# =========================
# 日本語フォント登録
# =========================
pdfmetrics.registerFont(UnicodeCIDFont("HeiseiMin-W3"))

# =========================
# 設定
# =========================
TOTAL_CHOICES = 5   # ★ 合計は常に5個
with open("English_words.txt", encoding="utf-8") as f:
    WORD_POOL = [line.strip() for line in f if line.strip()]

target_words = random.sample(WORD_POOL, 20)




# =========================
# ダミー生成
# =========================
def make_dummy(word):
    letters = list(word)
    i = random.randrange(len(letters))
    letters[i] = random.choice("abcdefghijklmnopqrstuvwxyz")
    return "".join(letters)

# =========================
# PDF設定
# =========================
file_name = f"word_scanning_{int(time.time())}.pdf"
doc = SimpleDocTemplate(file_name, pagesize=A4)

styles = getSampleStyleSheet()
styles["Normal"].fontName = "HeiseiMin-W3"
styles.add(ParagraphStyle(
    name="MyTitle",
    fontName="HeiseiMin-W3",
    fontSize=18,
    spaceAfter=15
))

story = []

# =========================
# タイトル
# =========================
story.append(Paragraph("単語スキャニング訓練（合計5個・正解数ランダム）", styles["MyTitle"]))
story.append(Paragraph(
    "各行には指定単語と同じ綴りの語が1〜3個含まれ、合計候補数は常に5個です。",
    styles["Normal"]
))
story.append(Spacer(1, 15))

# =========================
# 表データ生成
# =========================
table_data = []

for word in target_words:
    correct_count = random.randint(1, 3)
    dummy_count = TOTAL_CHOICES - correct_count

    # 正解語（重複OK）
    correct_words = [word] * correct_count

    # ダミー語（重複なし）
    dummy_words = set()
    while len(dummy_words) < dummy_count:
        dummy_words.add(make_dummy(word))

    candidates = correct_words + list(dummy_words)
    random.shuffle(candidates)

    row = [f"指定: {word}"] + candidates
    table_data.append(row)

# =========================
# 表描画
# =========================
table = Table(
    table_data,
    colWidths=[90] + [70] * TOTAL_CHOICES
)
story.append(table)

# =========================
# PDF生成
# =========================
doc.build(story)

print("PDFを生成しました:", file_name)
