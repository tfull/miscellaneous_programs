module Main where

import Control.Monad

maxN = 2020

odds :: [Integer]
odds = [1,3..]

main :: IO ()
main = do
    putStrLn "n: min k [nCk is even]"
    forM_ (take ((maxN + 1) `div` 2) odds) $ \n -> do
        putStrLn $ show n ++ ": " ++ show (loop n 1 1)

loop n k t =
    let nt = t * (n - k + 1) `div` k in
    if nt `mod` 2 == 0 then
        k
    else if n <= k then
        -1
    else
        loop n (k + 1) nt
