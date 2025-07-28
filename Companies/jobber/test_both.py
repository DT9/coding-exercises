import unittest
from semver import nextVersion
from spiral import createSpiral
class TreeBoth(unittest.TestCase):

    def test_version(self):
        self.assertEqual(nextVersion("1.2.3"),"1.2.4") 
        self.assertEqual(nextVersion("0.9.9"),"1.0.0") 
        self.assertEqual(nextVersion("1"),"2") 
        self.assertEqual(nextVersion("1.2.3.4.5.6.7.8"),"1.2.3.4.5.6.7.9")
        self.assertEqual(nextVersion("9.9"),"10.0")

    def test_spiral(self):
        self.assertEqual(createSpiral(3),[[1, 2, 3], [8, 9, 4], [7, 6, 5]])
        self.assertEqual(createSpiral(4),[[1, 2, 3, 4], [12, 13, 14, 5], [11, 16, 15, 6], [10, 9, 8, 7]])
        self.assertEqual(createSpiral(5),[[1, 2, 3, 4, 5], [16, 17, 18, 19, 6], [15, 24, 25, 20, 7], [14, 23, 22, 21, 8], [13, 12, 11, 10, 9]])
        
if __name__ == '__main__':
    unittest.main()



