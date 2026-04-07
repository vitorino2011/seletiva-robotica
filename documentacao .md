# 🌊 Sistema de Monitoramento de pH da Água

---

## 📌 Visão Geral

Este documento contém **toda a documentação do sistema** em um único arquivo `.md`, incluindo:

* Banco de dados (SQL)
* Conexão com Python
* Análise com Pandas
* Visualização gráfica

---

## 🧪 Sensor de pH

<div align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/5/5e/PH_meter.jpg" width="300"/>
  <br>
  <em>Sensor de pH utilizado para medir acidez/alcalinidade</em>
</div>

---

## 📊 Conceito de pH

* 0 – 6.4 → Ácido (Perigoso)
* 6.5 – 8.5 → Potável (Seguro)
* 8.6 – 14 → Alcalino (Perigoso)

---

## 🗄️ Banco de Dados (SQL)

```sql
CREATE DATABASE monitoramento_agua;

USE monitoramento_agua;

CREATE TABLE leituras (
    id INT AUTO_INCREMENT PRIMARY KEY,
    valor FLOAT,
    data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

INSERT INTO leituras (valor) VALUES
(6.8),(7.1),(6.9),(7.3),(8.0),(5.9),(6.5),(7.8),(8.2),(6.7),
(7.0),(6.6),(7.4),(8.1),(5.8),(6.2),(7.6),(8.4),(6.3),(7.2);
```

---

## 🐍 Conexão com Banco (Python)

```python
import mysql.connector


def conectar():
    return mysql.connector.connect(
        host="localhost",
        user="root",
        password="",
        database="monitoramento_agua"
    )


def obter_dados():
    conexao = conectar()
    cursor = conexao.cursor()

    cursor.execute("SELECT valor FROM leituras")
    dados = cursor.fetchall()

    cursor.close()
    conexao.close()

    return [d[0] for d in dados]
```

---

## 📊 Análise de Dados (Pandas)

```python
import pandas as pd
from banco_de_dados import obter_dados

valores = obter_dados()

df = pd.DataFrame(valores, columns=["pH"])
```

---

## 🧠 Classificação dos Dados

```python
def classificar(ph):
    if ph < 6.5:
        return "Perigoso (Ácido)"
    elif ph > 8.5:
        return "Perigoso (Alcalino)"
    else:
        return "Seguro"


df["Classificacao"] = df["pH"].apply(classificar)
```

---

## 📉 Ordenação

```python
df = df.sort_values(by="pH")
```

---

## 🎨 Gradiente de Cores

```python
import numpy as np

min_val = df["pH"].min()
max_val = df["pH"].max()

norm = (df["pH"] - min_val) / (max_val - min_val)

colors = []
for n in norm:
    r = 1 - (n * 0.5)
    g = 0
    b = n
    colors.append((r, g, b))
```

---

## 📈 Geração do Gráfico

```python
import matplotlib.pyplot as plt

plt.figure(figsize=(12, 6))

plt.bar(range(len(df)), df["pH"], color=colors)

plt.title("Nível de pH da Água (Perigoso → Seguro)")
plt.xlabel("Amostras")
plt.ylabel("pH")

plt.axhline(y=6.5)
plt.axhline(y=8.5)

plt.grid()
plt.show()
```

---

## 🎯 Interpretação Visual

<div align="center">
  <table>
    <tr>
      <td style="color:red;"><b>🔴 Vermelho</b></td>
      <td>Água Ácida (Perigosa)</td>
    </tr>
    <tr>
      <td style="color:purple;"><b>🟣 Roxo</b></td>
      <td>Água Segura</td>
    </tr>
  </table>
</div>

---

## 🔋 Sustentabilidade

O sistema pode ser alimentado por:

* Placa solar
* Bateria recarregável
* Regulador de tensão

---





