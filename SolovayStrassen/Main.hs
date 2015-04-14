module Main where

import System.Random

q1 :: Integer -> IO ()
q1 n = do
  b <- fSolovayStrassen n
  putStrLn $ show n ++ " is " ++ boolToSymbol b

q2 :: IO ()
q2 = q2s listMersenne
  where
    q2s :: [Integer] -> IO ()
    q2s [] = return ()
    q2s (n:ns) = do
      b <- fSolovayStrassen n
      putStrLn $ show n ++ " is " ++ boolToSymbol b
      q2s ns

q3 :: IO ()
q3 = loop (0, 0) $ makeG 2 []
  where
    makeG :: Integer -> [Integer] -> [Integer]
    makeG x xs
      | x > 256 = []
      | any (\z -> x `mod` z == 0) xs = x : makeG (x + 1) xs
      | otherwise = makeG (x + 1) (x : xs)
    loop :: (Integer, Integer) -> [Integer] -> IO ()
    loop (i, total) [] = putStrLn . show $ i * 100 `div` total
    loop (i, total) (x : xs) = do
      b <- fSolovayStrassen x
      if b then do
        putStrLn $ show x ++ " is prime. " ++ show (i, total + 1)
        loop (i, total + 1) xs
      else do
        putStrLn $ show x ++ " is not prime. " ++ show (i + 1, total + 1)
        loop (i + 1, total + 1) xs

boolToSymbol :: Bool -> String
boolToSymbol b = if b then "prime" else "not prime"

fSolovayStrassen :: Integer -> IO Bool
fSolovayStrassen n = do
  a <- (getStdRandom $ randomR (1, n - 1) :: IO Integer)
  if gcd a n /= 1 then return False
  else return $ (fJacobiSymbol a n - pmd a (half n) n) `mod` n == 0

listMersenne :: [Integer]
listMersenne = map (\n -> 2 ^ n - 1) [2..]

pmd :: Integer -> Integer -> Integer -> Integer
pmd a n p
  | n == 0 = 1
  | n == 1 = a
  | odd n = a * pmd a (n - 1) p `mod` p
  | otherwise = (pmd a (n `div` 2) p) ^ 2 `mod` p

half :: Integer -> Integer
half n = (n - 1) `div` 2

fJacobiSymbol :: Integer -> Integer -> Integer
fJacobiSymbol a m = f1 (a, m, 1)
  where
    f1 (a, m, t)
      | a /= 0 = f1 . f3 . f2 $ (a, m, t)
      | otherwise = if m == 1 then t else 0
    f2 (a, m, t)
      | a `mod` 2 == 0 =
        let w = if m `mod` 8 == 3 || m `mod` 8 == 5 then - t else t in
        f2 (a `div` 2, m, w)
      | otherwise = (a, m, t)
    f3 (m, a, t) =
      let x = if a `mod` 4 == 3 && m `mod` 4 == 3 then - t else t in
      (a `mod` m, m, x)
