This file and referenced files are on the address https://github.com/danielnager/xifrat/

We will use a substitution table, for example, a 13x13 one:

     5  3  0 12 11  4  9 10  8  1  6  7  2
     3  7  2 11  9 10  5  6  0 12  8  4  1
     0  2  3 10  6 12  8 11  5  4  9  1  7
    12 11 10  0  2  5  1  3  4  8  7  9  6
    11  9  6  2  1  3 12  7 10  0  4  5  8
     4 10 12  5  3  8  7  0  1  9  2  6 11
     9  5  8  1 12  7 11  4  6  2 10  3  0
    10  6 11  3  7  0  4  2 12  5  1  8  9
     8  0  5  4 10  1  6 12  9  7 11  2  3
     1 12  4  8  0  9  2  5  7  6  3 11 10
     6  8  9  7  4  2 10  1 11  3 12  0  5
     7  4  1  9  5  6  3  8  2 11  0 10 12
     2  1  7  6  8 11  0  9  3 10  5 12  4

to define a function c=f(a,b), where c is the element in the a-th row and b-th column.

The following two properties hold:

!= means different from

    f(f(a,b),c)!=f(a,f(b,c)) -- non-associativity in general
    f(f(a,b),f(c,d))=f(f(a,c),f(b,d)) -- restricted commutativity

Next we define a list of N numbers in the integer range [0,12] to meet the size required. For 256 bits we need 256/log2(13)=69 approximately. So let's set N=69. This list can be interpreted as a 69 digit base-13 number.

Next we define a mixing procedure of elements of this kind, t and k, N-element lists of numbers in the integer range [0,12].

The mixing procedure is:

    function m(t,k) returns t

        for M number of rounds -- 64 for example
            //one-to-one mixing of k and t
            for i in 0..N-1
                t[i]<-f(t[i],k[i])
            end for
            // accumulative mixing of t with itself, t[-1]=t[N-1]
            for i in 0..N-1
                t[i]<-f(t[i],t[i-1])
            end for
        end for

    return t

The function m is neither associative nor commutative, and meets the restricted commutativity property:

m(m(a,b),m(c,d))=m(m(a,c),m(b,d))

With this a Secret agreement and a Digital signature can be done as explained in the document:

https://github.com/danielnager/xifrat/raw/master/cryptosystem.pdf

The computationally hard problem proposed is:

in c=m(t,k), knowing c and t, find k.

Now lets define the secret agreement and the digital signature using the mixing function m. To put it more clear we will use the following notation:

m(a,b) is written as (a b)

m(m(a,b),m(c,d)) is written as (a b)(c d)

m(m(a,b),c) is written as (a b c)

For the secret agreement the procedure is the following:

Both Alice and Bob agree on some constant C. Alice chooses a random key K, and Bob does the same choosing a random key Q.
Alice sends to Bob (C K), Bob sends to Alice (C Q).
Alice computes using bob sent value (C Q)(K C), and Bob does the same and computes (C K)(Q C).

By the property of restricted commutativity (C Q)(K C)=(C K)(Q C)

For the signature the procedure is the following:

Alice, the signer, chooses a public value C and two different random keys K and Q. Its credentials are C, (C K) and (Q K).
To sing a value, H, Alice computes S=(H Q). 

Bob needs to verify if Alice has signed H. Computes (H Q)(C K) and (H C)(Q K). Both values must be equal due to restricted commutativity if (H Q) is a valid signature from Alice.

Daniel Nager - daniel.nager@gmail.com

