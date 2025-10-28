#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char *name;
    float price;
    int quantity;
} Product;

Product* initializeInventory(int totalProducts);
void addProduct(Product **inventory, int *totalProducts);
void viewProducts(Product *inventory, int totalProducts);
void updateQuantity(Product *inventory, int totalProducts);
void searchByID(Product *inventory, int totalProducts);
void searchByName(Product *inventory, int totalProducts);
void searchByPriceRange(Product *inventory, int totalProducts);
void deleteProduct(Product **inventory, int *totalProducts);
void freeInventory(Product *inventory, int totalProducts);

Product* initializeInventory(int totalProducts) {
    Product *ptr = (Product*)calloc(totalProducts, sizeof(Product));
    if (ptr == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    return ptr;
}

void addProduct(Product **inventory, int *totalProducts) {
    *totalProducts += 1;

    *inventory = (Product*)realloc(*inventory, (*totalProducts) * sizeof(Product));
    if (*inventory == NULL) {
        printf("Memory reallocation failed!\n");
        exit(1);
    }

    (*inventory)[*totalProducts - 1].name = (char*)malloc(50 * sizeof(char));
    if ((*inventory)[*totalProducts - 1].name == NULL) {
        printf("Memory allocation for name failed!\n");
        exit(1);
    }

    printf("\nEnter the new product details:\n");
    printf("product ID: ");
    scanf("%d", &(*inventory)[*totalProducts - 1].id);
    printf("Product Name: ");
    scanf("%s", (*inventory)[*totalProducts - 1].name);
    printf("product Price: ");
    scanf("%f", &(*inventory)[*totalProducts - 1].price);
    printf("Product Quantity: ");
    scanf("%d", &(*inventory)[*totalProducts - 1].quantity);

    printf("Product added Successfully!\n");
}

void viewProducts(Product *inventory, int totalProducts) {
    if (totalProducts == 0) {
        printf("\nNo products in inventory.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[productIndex].id,
               inventory[productIndex].name,
               inventory[productIndex].price,
               inventory[productIndex].quantity);
    }
}

void updateQuantity(Product *inventory, int totalProducts) {
    int productID, newQuantity, found = 0;
    printf("Enter Product ID to update: ");
    scanf("%d", &productID);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].id == productID) {
            printf("Enter new Quantity: ");
            scanf("%d", &newQuantity);
            inventory[productIndex].quantity = newQuantity;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("Product not found!\n");
}

void searchByID(Product *inventory, int totalProducts) {
    int productID, found = 0;
    printf("Enter Product ID to search: ");
    scanf("%d", &productID);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].id == productID) {
            printf("Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
            found = 1;
            break;
        }
    }
    if (!found) printf("Product not found!\n");
}

void searchByName(Product *inventory, int totalProducts) {
    char nameQuery[50];
    int found = 0;
    printf("Enter name to search (partial allowed): ");
    scanf("%s", nameQuery);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (strstr(inventory[productIndex].name, nameQuery)) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
            found = 1;
        }
    }
    if (!found) printf("No product matched.\n");
}

void searchByPriceRange(Product *inventory, int totalProducts) {
    float minPrice, maxPrice;
    int found = 0;

    printf("Enter minimum price: ");
    scanf("%f", &minPrice);
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        if (inventory[productIndex].price >= minPrice && inventory[productIndex].price <= maxPrice) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[productIndex].id,
                   inventory[productIndex].name,
                   inventory[productIndex].price,
                   inventory[productIndex].quantity);
            found = 1;
        }
    }

    if (!found) printf("No products found in range.\n");
}

void deleteProduct(Product **inventory, int *totalProducts) {
    int productID, indexToDelete = -1;

    printf("Enter Product ID to delete: ");
    scanf("%d", &productID);

    for (int productIndex = 0; productIndex < *totalProducts; productIndex++) {
        if ((*inventory)[productIndex].id == productID) {
            indexToDelete = productIndex;
            break;
        }
    }

    if (indexToDelete == -1) {
        printf("Product not found!\n");
        return;
    }

    free((*inventory)[indexToDelete].name);

    for (int productIndex = indexToDelete; productIndex < *totalProducts - 1; productIndex++) {
        (*inventory)[productIndex] = (*inventory)[productIndex + 1];
    }

    *totalProducts -= 1;

    *inventory = (Product*)realloc(*inventory, (*totalProducts) * sizeof(Product));
    if (*inventory == NULL && *totalProducts > 0) {
        printf("Memory reallocation failed after deletion!\n");
        exit(1);
    }

    printf("Product deleted successfully!\n");
}

void freeInventory(Product *inventory, int totalProducts) {
    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        free(inventory[productIndex].name);
    }
    free(inventory);
}

int main() {
    int totalProducts, choice;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    scanf("%d", &totalProducts);

    inventory = initializeInventory(totalProducts);

    for (int productIndex = 0; productIndex < totalProducts; productIndex++) {
        printf("\nEnter details for product %d:\n", productIndex + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[productIndex].id);

        inventory[productIndex].name = (char*)malloc(50 * sizeof(char));
        printf("Product Name: ");
        scanf("%s", inventory[productIndex].name);

        printf("Product Price: ");
        scanf("%f", &inventory[productIndex].price);

        printf("Product Quantity: ");
        scanf("%d", &inventory[productIndex].quantity);
    }

    while (1) {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct(&inventory, &totalProducts);
                break;
            case 2:
                viewProducts(inventory, totalProducts);
                break;
            case 3:
                updateQuantity(inventory, totalProducts);
                break;
            case 4:
                searchByID(inventory, totalProducts);
                break;
            case 5:
                searchByName(inventory, totalProducts);
                break;
            case 6:
                searchByPriceRange(inventory, totalProducts);
                break;
            case 7:
                deleteProduct(&inventory, &totalProducts);
                break;
            case 8:
                freeInventory(inventory, totalProducts);
                printf("\nMemory released successfully. Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
