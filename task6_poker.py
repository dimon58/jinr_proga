import enum
from collections import Counter
from dataclasses import dataclass
from typing import TypeAlias


class Value(enum.IntEnum):
    Ace = 14
    King = 13
    Queen = 12
    Jack = 11
    C10 = 10
    C9 = 9
    C8 = 8
    C7 = 7
    C6 = 6
    C5 = 5
    C4 = 4
    C3 = 3
    C2 = 2


class Suite(enum.IntEnum):
    Spades = 1
    Clubs = 2
    Hearts = 3
    Diamonds = 4


class CardCombination(enum.IntEnum):
    RoyalFlush = 10
    StraightFlush = 9
    FourOfKind = 8
    FullHouse = 7
    Flush = 6
    Straight = 5
    ThreeOfKind = 4
    TwoPairs = 3
    OnePair = 2
    HighCard = 1

    def __str__(self):
        return self.name

    def __repr__(self):
        return self.name


CombinationResult: TypeAlias = tuple[bool, Value]


@dataclass
class Card:
    value: Value
    suit: Suite

    def __init__(self, encoded):
        self.value = {
            "A": Value.Ace,
            "K": Value.King,
            "Q": Value.Queen,
            "J": Value.Jack,
            "T": Value.C10,
            "9": Value.C9,
            "8": Value.C8,
            "7": Value.C7,
            "6": Value.C6,
            "5": Value.C5,
            "4": Value.C4,
            "3": Value.C3,
            "2": Value.C2,
        }[encoded[:-1]]

        self.suit = {
            "S": Suite.Spades,
            "C": Suite.Clubs,
            "H": Suite.Hearts,
            "D": Suite.Diamonds,
        }[encoded[-1]]

    def __repr__(self):
        return f"{self.value.name}-{self.suit.name}"

    def __lt__(self, other):
        return self.value < other.value


class Hand:
    def __init__(self, cards: list[Card]):

        if len(cards) != 5:
            raise ValueError(f"Len of {cards} != 5")

        self.hand = sorted(cards, reverse=True)
        self.combination, self.combination_value = self.__combination()

    def __repr__(self):
        cards = ", ".join(map(str, self.hand))
        return f"Hand<{cards}>"

    def __gt__(self, other: "Hand"):
        if self.combination > other.combination:
            return True

        if self.combination == CardCombination.FullHouse:
            return (
                    Counter(map(lambda x: x.value, self.hand)).most_common(1)[0][0]
                    > Counter(map(lambda x: x.value, other.hand)).most_common(1)[0][0]
            )

        if self.combination_value != other.combination_value:
            return self.combination_value > other.combination_value

        for self_card, other_card in zip(self.hand, other.hand):
            if self_card.value == other_card.value:
                continue

            return self_card.value > other_card.value

        raise NotImplementedError("Ничья")

    def is_royal_flush(self) -> CombinationResult:
        """
        роял-флэш: туз, король, дама, валет, десять одной масти
        """
        return (
                       self.hand[0].value == Value.Ace
                       and self.hand[1].value == Value.King
                       and self.hand[2].value == Value.Queen
                       and self.hand[3].value == Value.Jack
                       and self.hand[4].value == Value.C10
                       and self.is_flush()
               ), self.hand[0].value

    def is_straight_flush(self) -> CombinationResult:
        """
        стрит-флэш: пять карт одной масти по порядку
        """

        s, _ = self.is_straight()
        f, _ = self.is_flush()

        return s and f, self.hand[0].value

    def is_four_of_kind(self) -> CombinationResult:
        """
        каре: четыре карты одного достоинства
        """
        c = Counter(map(lambda x: x.value, self.hand)).most_common(1)[0]
        return c[1] == 4, c[0]

    def is_full_house(self) -> CombinationResult:
        """
        фулл-хаус: одна тройка и одна пара
        """
        c = Counter(map(lambda x: x.value, self.hand)).most_common(2)
        return c[0][1] == 3 and c[1][1] == 2, c[0][0]

    def is_flush(self) -> CombinationResult:
        """
        флэш: пять карт одной масти
        """
        return (
                       self.hand[0].suit == self.hand[1].suit
                       and self.hand[0].suit == self.hand[2].suit
                       and self.hand[0].suit == self.hand[3].suit
                       and self.hand[0].suit == self.hand[4].suit
               ), self.hand[0].value

    def is_straight(self) -> CombinationResult:
        """
        стрит: пять карт по порядку любых мастей
        """
        return (
                       self.hand[0].value - self.hand[1].value == 1
                       and self.hand[1].value - self.hand[2].value == 1
                       and self.hand[2].value - self.hand[3].value == 1
                       and self.hand[3].value - self.hand[4].value == 1
               ), self.hand[0].value

    def is_three_of_kind(self) -> CombinationResult:
        """
        тройка: три карты одного достоинства
        """
        c = Counter(map(lambda x: x.value, self.hand)).most_common(1)[0]
        return c[1] == 3, c[0]

    def is_two_pair(self) -> CombinationResult:
        """
        две пары: две разные пары
        """
        c = Counter(map(lambda x: x.value, self.hand)).most_common(2)
        return c[0][1] == 2 and c[1][1] == 2, c[0][0]

    def is_one_pair(self) -> CombinationResult:
        """
        пара: две карты одного достоинства
        """
        c = Counter(map(lambda x: x.value, self.hand)).most_common(1)[0]
        return c[1] == 2, c[0]

    def __combination(self) -> tuple[CardCombination, Value]:

        for combination, condition in (
                (CardCombination.RoyalFlush, self.is_royal_flush),
                (CardCombination.StraightFlush, self.is_straight_flush),
                (CardCombination.FourOfKind, self.is_four_of_kind),
                (CardCombination.FullHouse, self.is_full_house),
                (CardCombination.Flush, self.is_flush),
                (CardCombination.Straight, self.is_straight),
                (CardCombination.ThreeOfKind, self.is_three_of_kind),
                (CardCombination.TwoPairs, self.is_two_pair),
                (CardCombination.OnePair, self.is_one_pair),
        ):
            is_true, value = condition()
            if is_true:
                return combination, value

        return CardCombination.HighCard, self.hand[0].value


def parse_line(line: str) -> tuple[Hand, Hand]:
    tmp = line.strip().split()

    return Hand([Card(v) for v in tmp[:5]]), Hand([Card(v) for v in tmp[5:]])


def test():
    hand1, hand2 = parse_line("5H 5C 6S 7S KD 2C 3S 8S 8D TD")
    assert hand1.combination == CardCombination.OnePair
    assert hand2.combination == CardCombination.OnePair
    assert hand1 < hand2

    hand1, hand2 = parse_line("5D 8C 9S JS AC 2C 5C 7D 8S QH")
    assert hand1.combination == CardCombination.HighCard
    assert hand2.combination == CardCombination.HighCard
    assert hand1 > hand2

    hand1, hand2 = parse_line("2D 9C AS AH AC 3D 6D 7D TD QD")
    assert hand1.combination == CardCombination.ThreeOfKind
    assert hand2.combination == CardCombination.Flush
    assert hand1 < hand2

    hand1, hand2 = parse_line("4D 6S 9H QH QC 3D 6D 7H QD QS")
    assert hand1.combination == CardCombination.OnePair
    assert hand2.combination == CardCombination.OnePair
    assert hand1 > hand2

    hand1, hand2 = parse_line("2H 2D 4C 4D 4S 3C 3D 3S 9S 9D")
    assert hand1.combination == CardCombination.FullHouse
    assert hand2.combination == CardCombination.FullHouse
    assert hand1 > hand2


def main():
    test()

    victories = 0
    with open("tasks/poker.txt", "r") as file:
        for i in file.readlines():
            hand1, hand2 = parse_line(i)
            if hand1 > hand2:
                victories += 1
    print(f"First player won {victories} times")


if __name__ == '__main__':
    main()
