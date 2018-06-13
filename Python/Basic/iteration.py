"""
    Concept of iterable and iterator.
"""

# iterable object refers the object you request iteration for, whose __iter__ is run by iter.
# iterator object refers the object returned by the iterable that actually produces values
# during the iteration, whose __next__ is run by next and raises StopIteration when finished
# producing results.

#####################
# list as iterable.
#####################
list1 = [1, 2, 3]  # list is iterable object. does not have __next__ attribute.
iterator = list1.__iter__()  # iterator object has __next__
result = iterator.__next__()

print(iterator)
print(result)

# equal to call object's __iter__ and __next__
iterator = iter(list1)
result = next(iterator)

print(iterator)
print(result)

#####################
# file object is its own iterator. so it is both iterable and iterator.
#####################
file = open('iteration.py')
print(file, iter(file))
assert file is iter(file)

#####################
# dict as iterable
#####################
dict1 = {'a':1, 'b':2, 'c':3}
# iterate keys list as iterable, no need now.
for key in dict1.keys():
    print(key, dict1[key])
# iterate dict as iterable directly.
for key in dict1:
    print(key, dict1[key])
