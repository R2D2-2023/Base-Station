import psycopg2
import json

def main():
    login = open("login.hidden")
    conn = psycopg2.connect(
        host=str(login.readline().strip()),
        database=str(login.readline().strip()),
        user=str(login.readline().strip()),
        password=str(login.readline().strip()),
        port=str(login.readline().strip()))

    cur = conn.cursor()

    cur.execute("SELECT voltage FROM \"voltage-robot\" ORDER BY datetime DESC LIMIT 1;")
    voltage = cur.fetchall()

    voltage_dict = {}

    voltage_dict["voltage"] = voltage[0][0]

    return json.dumps(voltage_dict)

if __name__ == '__main__':
    print(main())