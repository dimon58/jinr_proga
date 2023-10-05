import time


def collatz_generator(n: int, max_iters: int = 10000):
    """
    :param n: стартовое число
    :param max_iters: максимальное число итераций
    """
    start = n
    for i in range(max_iters):
        yield n
        if n == 1:
            return
        if n % 2 == 0:
            n = n // 2
        else:
            n = 3 * n + 1

    print(f"!!!!!!!!!!! Max iterations ({max_iters}) reached for number {start}")


def main():
    max_length = 0
    max_length_number = 0

    max_value = 0
    max_value_number = 0

    total = 1_000_000
    s = time.perf_counter()
    for n in range(1, total + 1):
        if n % 100000 == 0:
            print(f"Checked {n}/{total}, {n / (time.perf_counter() - s):.0f} numbers/sec")
        length = 0
        for i in collatz_generator(n):
            length += 1

            if i > max_value:
                max_value = i
                max_value_number = n

        if length > max_length:
            max_length = length
            max_length_number = n
        max_length = max(max_length, length)

    e = time.perf_counter()

    print(f"Max length = {max_length} for number {max_length_number}")
    print(f"Max value = {max_value} for number {max_value_number}")
    print(f"Done in {e - s:.1f} sec ({total / (e - s):.0f} numbers/sec)")


if __name__ == '__main__':
    main()
