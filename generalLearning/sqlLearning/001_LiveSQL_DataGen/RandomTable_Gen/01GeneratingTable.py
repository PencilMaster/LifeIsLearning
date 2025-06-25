import sys                  # We will use this to define the amount of entries we want to be generated
import argparse             # We use parser vor clearer usability

from faker import Faker
import random
import datetime

fake = Faker()

# Structure is following:
# index, transaction_id, customer_name, product_id, quantity, total_cost, transaction_date

# We will Increment over Index in creation loop to keep them unique

def generate_transaction_id(index):
    return index

def generate_customer_name():
    return fake.name()

def generate_product_id():
    return random.randint(1, 1000)

def generate_quantity():
    return random.randint(1, 10)

def generate_total_cost(product_id, quantity):
    unit_price = random.Random(product_id).uniform(5.0, 500.0)
    return round(quantity * unit_price, 2)

def generate_transaction_date():
    today = datetime.date.today()
    days_offset = random.randint(0, 90)
    random_date = today - datetime.timedelta(days=days_offset)
    return random_date.isoformat()      # This translates to '2025-05-01'

def generate_record(index):
    product_id = generate_product_id()
    quantity = generate_quantity()
    return f"{generate_transaction_id(index):<6}, {generate_customer_name():<20}, {product_id:<6}, {quantity:<3}, {generate_total_cost(product_id,quantity):<8}, {generate_transaction_date()}"
    

def main():
    # We define our argument parser here
    parser = argparse.ArgumentParser()
    parser.add_argument('--records', type=int, default=100)
    parser.add_argument('--name', type=str, default='TableRandomGenDefault')
    args = parser.parse_args()
    recordAmount = args.records
    recordName = args.name

    print(f"Generating {recordName} {recordAmount} records...")

    with open(f'{recordName}.txt', 'w') as file:
        file.write("transaction_id, customer_name, product_id, quantity, total_cost, transaction_date \n")
        for number in range(recordAmount):
            file.write(generate_record(number + 1) + "\n")
            if (number / recordAmount == 0.25): 
                print("generated 25%")
            elif (number / recordAmount == 0.5): 
                print("generated 50%")
            elif (number / recordAmount == 0.75): 
                print("generated 75%")

    return

if __name__ == "__main__":
    main()
