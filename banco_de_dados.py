import sqlite3
from dados_ph import VALORES_PH

NOME_BANCO = "monitoramento_agua.db"


def conectar():
    return sqlite3.connect(NOME_BANCO)


def criar_tabela():
    conexao = conectar()
    cursor = conexao.cursor()

    cursor.execute("""
        CREATE TABLE IF NOT EXISTS leituras (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            valor REAL NOT NULL,
            data_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    """)

    conexao.commit()
    cursor.close()
    conexao.close()


def limpar_tabela():
    conexao = conectar()
    cursor = conexao.cursor()
    cursor.execute("DELETE FROM leituras")
    conexao.commit()
    cursor.close()
    conexao.close()


def inserir_dados():
    conexao = conectar()
    cursor = conexao.cursor()

    cursor.executemany(
        "INSERT INTO leituras (valor) VALUES (?)",
        [(valor,) for valor in VALORES_PH]
    )

    conexao.commit()
    cursor.close()
    conexao.close()


def obter_dados():
    conexao = conectar()
    cursor = conexao.cursor()

    cursor.execute("SELECT valor FROM leituras")
    dados = cursor.fetchall()

    cursor.close()
    conexao.close()

    return [dado[0] for dado in dados]


if __name__ == "__main__":
    criar_tabela()
    limpar_tabela()
    inserir_dados()
    print("Banco SQLite criado e dados inseridos com sucesso.")
