import math


def get_digit_list(n: int) -> list[int]:
    res = []
    while n > 0:
        res.append(n % 10)
        n //= 10

    return res[::-1]


def list_of_digits_to_number(digits: list[int]) -> int:
    res = 0
    for d in digits:
        res = res * 10 + d

    return res


def is_prime(n: int) -> bool:
    if n == 1:
        return False

    for i in range(2, math.trunc(math.sqrt(n)) + 1):
        if n % i == 0:
            return False

    return True


def get_circular_permutations(digits: list[int]):
    for i in range(len(digits)):
        yield digits[i:] + digits[:i]


def is_circular_prime(n: int) -> bool:
    digits = get_digit_list(n)

    if len(digits) > 1:
        for x in digits:
            if x in {0, 2, 4, 5, 6, 8}:
                return False

    for permutation in get_circular_permutations(digits):
        if not is_prime(list_of_digits_to_number(permutation)):
            return False

    return True


def print_cpn_before(n: int) -> None:
    for i in range(1, n):
        if is_circular_prime(i):
            print("Cpn", i)


def count_cpn_before(n: int) -> int:
    count = 0
    for i in range(1, n + 1):
        if i % 100000 == 0:
            print(f"Checked {i}/{n} numbers")
        if is_circular_prime(i):
            count += 1

    return count


def test():
    assert list_of_digits_to_number([1, 2, 3, 4]) == 1234
    assert is_prime(2)
    assert is_prime(3)
    assert is_prime(31)
    assert not is_prime(16)
    assert not is_prime(25)
    assert not is_prime(100)
    assert is_circular_prime(197), "197 должно быть круговым простым числом"
    assert is_circular_prime(719), "719 должно быть круговым простым числом"
    assert is_circular_prime(971), "971 должно быть круговым простым числом"
    assert count_cpn_before(100) == 13


def main():
    test()
    print_cpn_before(100)
    print(count_cpn_before(1_000_000))


if __name__ == "__main__":
    main()
