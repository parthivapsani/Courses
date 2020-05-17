import unittest
from client3 import *


class ClientTest(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(ClientTest, self).__init__(*args, **kwargs)
        self.quotes = [
            {'top_ask': {'price': 121.2, 'size': 36}, 'timestamp': '2019-02-11 22:06:30.572453', 'top_bid': {'price': 120.48, 'size': 109}, 'id': '0.109974697771', 'stock': 'ABC'},
            {'top_ask': {'price': 121.68, 'size': 4}, 'timestamp': '2019-02-11 22:06:30.572453', 'top_bid': {'price': 117.87, 'size': 81}, 'id': '0.109974697771', 'stock': 'DEF'}
        ]

    def test_getDataPoint_calculatePrice(self):
        for index, quote in enumerate(self.quotes):
            self.assertEqual(getDataPoint(quote)[3], (quote['top_ask']['price'] + quote['top_bid']['price']) / 2, "Quote {} price check failed".format(index + 1))

    def test_getDataPoint_calculatePriceBidGreaterThanAsk(self):
        for index, quote in enumerate(self.quotes):
            self.assertEqual(getDataPoint(quote)[3], (quote['top_ask']['price'] + quote['top_bid']['price']) / 2, "Quote {} greater than ask failed".format(index + 1))

    def test_getRatio_calculateRatio(self):
        if len(self.quotes) > 1:
            for index in range(len(self.quotes) - 1):
                self.assertEqual(getRatio(getDataPoint(self.quotes[index])[3], getDataPoint(self.quotes[index + 1])[3]),
                                 ((self.quotes[index]['top_ask']['price'] + self.quotes[index]['top_bid']['price']) / 2) / (
                                         (self.quotes[index + 1]['top_ask']['price'] + self.quotes[index + 1]['top_bid']['price']) / 2),
                                 "Quotes {} & {} ratioCheck failed".format(index, index + 1))


if __name__ == '__main__':
    unittest.main()
