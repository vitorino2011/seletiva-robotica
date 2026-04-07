import mysql.connector

def conectar():
    conexao = mysql.connector.connect(
        host="localhost",
        user="root",
        password="",
        database="monitoramento_agua"
    )
  
    return conexao


def obter_dados():
    conexao = conectar()
    cursor = conexao.cursor()

    cursor.execute("SELECT valor FROM leituras")
    dados = cursor.fetchall()

    cursor.close()
    conexao.close()

    # transformar em lista simples
    return [dado[0] for dado in dados]
