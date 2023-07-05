import logging
import psycopg2
import json

def main():
	conn = psycopg2.connect(
		host="m3s-sql-02.postgres.database.azure.com",
		database="postgres",
		user="m3s_admin",
		password="",
		port="5432")
		
	cur = conn.cursor()
	
	cur.execute("SELECT voltage FROM \"voltage-robot\" ORDER BY datetime DESC LIMIT 1;")
	voltage = cur.fetchall()
	voltage_dict = {}
	voltage_dict["voltage"] = voltage[0][0]
	
	with open('/home/pi/basedstation/voltage.txt', 'w') as f: f.write(json.dumps(voltage_dict["voltage"]))

main()
