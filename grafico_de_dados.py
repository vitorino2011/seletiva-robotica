import pandas as pd
import matplotlib.pyplot as plt
from banco_de_dados import obter_dados
import numpy as np

# =========================
# PEGAR DADOS DO SQL
# =========================
valores = obter_dados()

# Criar DataFrame
df = pd.DataFrame(valores, columns=["pH"])

# =========================
# CLASSIFICAÇÃO
# =========================
def classificar(ph):
    if ph < 6.5:
        return "Acido (Perigoso)"
    elif ph > 8.5:
        return "Alcalino (Perigoso)"
    else:
        return "Seguro"

df["Classificacao"] = df["pH"].apply(classificar)

# =========================
# ORDENAR (mais perigoso → mais seguro)
# =========================
df = df.sort_values(by="pH")

# =========================
# NORMALIZAÇÃO PARA CORES
# =========================
# Valores baixos → vermelho | altos → roxo
min_val = df["pH"].min()
max_val = df["pH"].max()

norm = (df["pH"] - min_val) / (max_val - min_val)

# Criar gradiente manual (vermelho → roxo)
colors = []
for n in norm:
    r = 1 - (n * 0.5)   # diminui vermelho
    g = 0               # mantém zero
    b = n               # aumenta azul → roxo
    colors.append((r, g, b))

# =========================
# GRÁFICO
# =========================
plt.figure(figsize=(12, 6))

plt.bar(range(len(df)), df["pH"], color=colors)

plt.title("Nível de pH da Água (Perigoso → Seguro)")
plt.xlabel("Amostras")
plt.ylabel("pH")

# Linhas de referência
plt.axhline(y=6.5, linestyle='--', label='Limite Inferior (Perigoso)')
plt.axhline(y=8.5, linestyle='--', label='Limite Superior (Perigoso)')

# Legenda manual (explicação das cores)
from matplotlib.patches import Patch
legend_elements = [
    Patch(facecolor=(1, 0, 0), label='Perigoso (Ácido)'),
    Patch(facecolor=(0.5, 0, 0.5), label='Seguro'),
    Patch(facecolor=(0.7, 0, 1), label='Alcalino')
]

plt.legend(handles=legend_elements)

plt.grid()

plt.show()
