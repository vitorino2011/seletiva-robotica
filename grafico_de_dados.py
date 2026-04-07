from flask import Flask
import pandas as pd
import matplotlib.pyplot as plt
from banco_de_dados import obter_dados
import numpy as np
import base64
from io import BytesIO
from matplotlib.patches import Patch

app = Flask(__name__)


@app.route("/")
def grafico():
    try:
        valores = obter_dados()

        if not valores:
            return """
            <html>
                <head>
                    <meta charset="UTF-8">
                    <title>Monitoramento de pH</title>
                </head>
                <body>
                    <h1>Sem dados no banco.</h1>
                    <p>Execute primeiro o arquivo <strong>banco_de_dados.py</strong>.</p>
                </body>
            </html>
            """

        df = pd.DataFrame(valores, columns=["pH"])

        def classificar(ph):
            if ph < 6.5:
                return "Ácido (Perigoso)"
            elif ph > 8.5:
                return "Alcalino (Perigoso)"
            else:
                return "Seguro"

        df["Classificacao"] = df["pH"].apply(classificar)
        df = df.sort_values(by="pH").reset_index(drop=True)

        min_val = df["pH"].min()
        max_val = df["pH"].max()

        if max_val == min_val:
            norm = np.zeros(len(df))
        else:
            norm = (df["pH"] - min_val) / (max_val - min_val)

        cores = []
        for n in norm:
            if n < 0.5:
                # tons mais próximos do vermelho/roxo
                r = 1 - (n * 0.5)
                g = 0
                b = n
            else:
                # tons mais próximos do roxo/azulado
                r = 0.75 - (n * 0.25)
                g = 0
                b = 0.5 + (n * 0.5)
            cores.append((r, g, b))

        plt.figure(figsize=(12, 6))
        plt.bar(range(len(df)), df["pH"], color=cores)

        plt.title("Nível de pH da Água")
        plt.xlabel("Amostras")
        plt.ylabel("pH")
        plt.axhline(y=6.5, linestyle="--", label="Limite inferior seguro")
        plt.axhline(y=8.5, linestyle="--", label="Limite superior seguro")
        plt.grid(True, axis="y", linestyle="--", alpha=0.6)

        legend_elements = [
            Patch(facecolor=(1, 0, 0), label="Ácido / Perigoso"),
            Patch(facecolor=(0.5, 0, 0.5), label="Faixa Segura"),
            Patch(facecolor=(0.4, 0, 1), label="Alcalino")
        ]

        plt.legend(handles=legend_elements)

        buffer = BytesIO()
        plt.savefig(buffer, format="png", bbox_inches="tight")
        buffer.seek(0)

        imagem_base64 = base64.b64encode(buffer.getvalue()).decode("utf-8")
        plt.close()

        media_ph = round(df["pH"].mean(), 2)
        menor_ph = round(df["pH"].min(), 2)
        maior_ph = round(df["pH"].max(), 2)

        return f"""
        <html>
            <head>
                <meta charset="UTF-8">
                <title>Monitoramento de pH</title>
                <style>
                    body {{
                        font-family: Arial, sans-serif;
                        text-align: center;
                        margin: 30px;
                        background-color: #f4f6f8;
                    }}
                    h1 {{
                        color: #222;
                    }}
                    .info {{
                        margin: 20px auto;
                        padding: 15px;
                        width: 60%;
                        background: white;
                        border-radius: 10px;
                        box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
                    }}
                    img {{
                        margin-top: 20px;
                        max-width: 95%;
                        height: auto;
                        border-radius: 10px;
                        box-shadow: 0 2px 8px rgba(0, 0, 0, 0.15);
                    }}
                </style>
            </head>
            <body>
                <h1>Gráfico de pH da Água</h1>

                <div class="info">
                    <p><strong>Total de amostras:</strong> {len(df)}</p>
                    <p><strong>Média do pH:</strong> {media_ph}</p>
                    <p><strong>Menor valor:</strong> {menor_ph}</p>
                    <p><strong>Maior valor:</strong> {maior_ph}</p>
                </div>

                <img src="data:image/png;base64,{imagem_base64}" alt="Gráfico de pH">
            </body>
        </html>
        """

    except Exception as e:
        return f"""
        <html>
            <head>
                <meta charset="UTF-8">
                <title>Erro</title>
            </head>
            <body>
                <h1>Erro ao gerar gráfico</h1>
                <p>{str(e)}</p>
            </body>
        </html>
        """


if __name__ == "__main__":
    print("Servidor iniciado em http://127.0.0.1:5000")
    app.run(debug=False)
