def forma_OEF(A):
    m, n = len(A), len(A[0])  # Dimensiones de la matriz

    fila_actual = 0  # Mantén un seguimiento de la fila actual que estás procesando

    for columna in range(n):
        # Encuentra el índice de la primera fila con una entrada no cero en la columna actual
        fila_no_cero = fila_actual
        while fila_no_cero < m and A[fila_no_cero][columna] == 0:
            fila_no_cero += 1

        # Si todas las entradas son cero, pasa a la siguiente columna
        if fila_no_cero == m:
            continue

        # Intercambia filas para asegurar que la primera entrada no cero esté en la fila_actual
        A[fila_actual], A[fila_no_cero] = A[fila_no_cero], A[fila_actual]

        # Normaliza la fila actual dividiendo por el primer elemento no cero
        pivot = A[fila_actual][columna]
        A[fila_actual] = [elem / pivot for elem in A[fila_actual]]

        # Hace cero todas las demás entradas en la columna actual
        for otra_fila in range(m):
            if otra_fila != fila_actual:
                factor = A[otra_fila][columna]
                A[otra_fila] = [elem - factor * A[fila_actual][i] for i, elem in enumerate(A[otra_fila])]

        # Mueve a la siguiente fila para procesar
        fila_actual += 1

    return A

# Ejemplo de uso
matriz_A = [
    [2, 1, -1, 8],
    [-3, -1, 2, -11],
    [-2, 1, 2, -3]
]

resultado_OEF = forma_OEF(matriz_A)
print(resultado_OEF)
