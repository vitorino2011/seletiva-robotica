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
<img width="1536" height="1024" alt="Image" src="https://github.com/user-attachments/assets/ec309c1f-b7a2-4614-9b03-ea8c717f5983" />
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
<div>
  <img width="792" height="650" alt="Image" src="https://github.com/user-attachments/assets/2d260aec-a2b6-41bb-85a4-e7ec81572e23" />
</div>

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
# 📘 Sistema de Monitoramento de Qualidade da Água com Arduino

## 📌 Visão Geral do Projeto

Este projeto consiste em um sistema automatizado de monitoramento da qualidade da água, com foco na medição de pH, sinalização visual e exibição de dados em tempo real. A arquitetura foi pensada para ser funcional, lógica e tecnicamente coerente, evitando ligações aleatórias e priorizando organização — exatamente como um bom circuito deve ser.

A proposta integra geração de energia, sensoriamento, processamento e interface com o usuário, formando um ecossistema completo e didático.

---

## Possivel circuto a ser ultilizado
<div>
<img width="1536" height="1024" alt="Image" src="https://github.com/user-attachments/assets/6149b0ff-c3d5-4ccc-9854-cd7fca1d7bb1" />
</div>

## ⚙️ Componentes Utilizados

### 🔹 1. Arduino (Microcontrolador)

O Arduino é o “cérebro” do circuito. Ele recebe os dados do sensor de pH, processa essas informações e toma decisões com base em parâmetros previamente definidos.

**Função principal:**

* Ler sinais analógicos (sensor de pH)
* Controlar saídas digitais (LEDs)
* Comunicar-se com o display LCD via protocolo I2C

**Justificativa técnica:**
A escolha do Arduino se dá pela sua versatilidade, baixo custo e facilidade de integração com sensores e módulos diversos. Para projetos educacionais ou protótipos, ele resolve sem drama — e sem gambiarra.

---

### 🔹 2. Sensor de pH

Responsável por medir a acidez ou alcalinidade da água, fornecendo um sinal analógico proporcional ao valor de pH.

**Faixa típica:**

* 0 a 14 (ácido → neutro → básico)

**Função no sistema:**

* Enviar dados contínuos ao Arduino
* Permitir análise da qualidade da água

**Justificativa:**
Sem medição, não há monitoramento. Esse sensor é o elemento central do projeto, pois define o comportamento de todo o restante do sistema.

---

### 🔹 3. Display LCD com Interface I2C

Utilizado para exibir os valores de pH e mensagens informativas ao usuário.

**Características:**

* Comunicação via I2C (reduz número de fios)
* Exibição em tempo real

**Função:**

* Mostrar dados coletados
* Informar status do sistema

**Justificativa:**
A interface I2C foi escolhida por economia de pinos e organização do circuito. Menos fios, menos confusão — simples assim.

---

### 🔹 4. LEDs (Verde e Vermelho)

Atuam como indicadores visuais do estado da água.

**Função:**

* 🔴 LED Vermelho: alerta de pH fora do padrão
* 🟢 LED Verde: condição segura/ideal

**Justificativa:**
Feedback visual imediato é essencial. Nem sempre o usuário vai ler o display — mas uma luz vermelha acesa chama atenção na hora.

---

### 🔹 5. Resistores

Associados aos LEDs para limitar a corrente elétrica.

**Função:**

* Evitar que os LEDs queimem
* Controlar intensidade da corrente

**Justificativa:**
Ignorar resistor é pedir pra queimar componente. Aqui não tem espaço para “ligar direto e ver no que dá”.

---

### 🔹 6. Placa Solar

Responsável por fornecer energia ao sistema, promovendo autonomia energética.

**Função:**

* Alimentar o circuito de forma sustentável
* Reduzir dependência de fontes externas

**Justificativa:**
Ideal para aplicações em campo ou locais remotos. Além disso, agrega valor ecológico ao projeto.

---

### 🔹 7. Protoboard (Breadboard)

Base de montagem do circuito.

**Função:**

* Permitir conexões sem solda
* Facilitar testes e alterações

**Justificativa:**
Ambiente controlado para prototipagem. Organização aqui evita dor de cabeça depois.

---

### 🔹 8. Jumpers (Fios de Conexão)

Realizam as interligações entre os componentes.

**Função:**

* Conectar módulos de forma lógica
* Transportar sinais e energia

**Boas práticas adotadas:**

* Cores padronizadas (vermelho = VCC, preto = GND, etc.)
* Fios com destino definido (sem “pontas soltas”)
* Organização paralela (evitando cruzamentos desnecessários)

**Justificativa:**
Circuito bem feito não é só funcional — ele é legível. Aqui, estética e técnica andam juntas.

---

## 🔌 Lógica de Funcionamento

1. A placa solar fornece energia ao sistema.
2. O Arduino inicializa e começa a leitura do sensor de pH.
3. O valor lido é processado e convertido.
4. O resultado é exibido no LCD.
5. Com base no valor:

   * Se estiver dentro do intervalo ideal → LED verde acende
   * Caso contrário → LED vermelho é acionado
6. O sistema opera continuamente, em tempo real.

---


