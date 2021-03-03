This file and referenced files are on the address https://github.com/danielnager/xifrat/

The aim of this cryptosystem is to provide post-quantum asymmetric cryptography with 256-bit sizes.

We will use a substitution 16x16 table, this is one of them:

     7  9 13 10 15  2  0  6  3 12  8  4  1  5 14 11 
     1 15  6  3  9  4 11 13 10  5 14  2  7 12  8  0 
     3  0 12  1 11  8  9  5  7 13  2 14 10  6  4 15 
     4  6 15  8 13  1  5  9 14 11 10  7  2  0  3 12 
     0  3  8 15 10 12  7 14  9  2 13  5 11  4  6  1 
    10 11  5  7  0 14 15 12  1  6  4  8  3 13  2  9 
     5 14 10 13  8 11  4  3  6  1 15  0 12  7  9  2 
    15  1  4  0  7  6 10  2 11 14  5 13  9  8 12  3 
    12  8  3  6 14  0  2 10 13  7  9 11  5  1 15  4 
    13  2  7  5  4  9  8  1 12  3  0 15  6 10 11 14 
     6  4  1 12  2 15 14  7  5 10 11  9 13  3  0  8 
     9  7  2 11  1 13  3  4  0  8 12  6 15 14  5 10 
    11 10 14  9  3  5  1  8 15  4  6 12  0  2 13  7 
    14  5 11  2 12 10  6  0  4 15  1  3  8  9  7 13 
     8 12  0  4  5  3 13 11  2  9  7 10 14 15  1  6 
     2 13  9 14  6  7 12 15  8  0  3  1  4 11 10  5

to define a function c=f(a,b), where c is the element in the a-th row and b-th column.

The following two properties hold:

!= means different from

    f(f(a,b),c)!=f(a,f(b,c)) -- non-associativity in general
    f(a,b)!=f(b,a) -- non-commutativity in general
    f(f(a,b),f(c,d))=f(f(a,c),f(b,d)) -- restricted commutativity

Next we define a list of N numbers in the integer range [0,15] to meet the size required. For 256 bits we need 64. So let's set N=64. This list can be interpreted as a 64 digit base-16 number.

Next we define a mixing procedure of elements of this kind, t and k, N-element lists of numbers in the integer range [0,15].

But before, we define a deterministic sequence obtained by a prng, for example, that returns always the same sequence of numbers in the interval [0,N-1]. frist\_seq gets the first element of the sequence and next\_seq the next one.

The mixing procedure is:

    function m(t,k) returns r

        //one-to-one mixing of k and t
        for i in 0..N-1
            r[i] <- f(t[i],k[i])
        end for
        i <- first_seq
        for M number of applications of f -- 4096 for example
            // accumulative mixing of r with itself
            j <- next_seq
            r[j]<-f(r[j],r[i])
            i <- j
        end for
        //one-to-one mixing of k and r
        for i in 0..N-1
            r[i] <- f(r[i],k[i])
        end for

    return r

The function m is neither associative nor commutative, and meets the restricted commutativity property:

m(m(a,b),m(c,d))=m(m(a,c),m(b,d))

With this a Secret agreement and a Digital signature can be done as explained in the document:

https://github.com/danielnager/xifrat/raw/master/cryptosystem.pdf

The computationally hard problem proposed is:

in c=m(t,k), knowing c and t, find k.

At this point an stop should be made to talk about differential and linear cryptoanalysis. If we take each row and each column as a 16 4-bit lenght substitution table, i turns out that the probability of a linear equation holding is 14/16 and more or less is the same for differential characteristics. As we're doing 4096 iteration on the s-table, despite 14/16 is a high probability, log2((14/16)^4096) is by far lesser than 2^-128. So there's no attack feasible here.

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

