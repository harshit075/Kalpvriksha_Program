#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 50

typedef struct {
    int id;
    char *name;
    float price;
    int quantity;
} Product;


Product* initializeInventory(int totalProducts);
int addProduct(Product **inventory, int *totalProducts);
void viewProducts(Product *inventory, int totalProducts);
void updateQuantity(Product *inventory, int totalProducts);
void searchByID(Product *inventory, int totalProducts);
void searchByName(Product *inventory, int totalProducts);
void searchByPriceRange(Product *inventory, int totalProducts);
int deleteProduct(Product **inventory, int *totalProducts);
void freeInventory(Product *inventory, int totalProducts);


Product* initializeInventory(int totalProducts) {
    if (totalProducts <= 0) {
        printf("No initial products.\n");
        return NULL;
    }

    Product *ptr = calloc(totalProducts, sizeof(Product));
    if (!ptr) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    return ptr;
}

int addProduct(Product **inventory, int *totalProducts) {
    int newCount = *totalProducts + 1;

    Product *temp = realloc(*inventory, newCount * sizeof(Product));
    if (temp == NULL) {
        printf("Memory reallocation failed! Product not added.\n");
        return 0;
    }

    *inventory = temp;
    Product *newProd = &((*inventory)[newCount - 1]);

    newProd->name = malloc(NAME_SIZE);
    if (newProd->name == NULL) {
        printf("Memory allocation for name failed!\n");
        return 0;
    }

    printf("\nEnter new product details:\n");
    printf("Product ID: ");
    scanf("%d", &newProd->id);
    printf("Product Name: ");
    scanf("%49s", newProd->name);
    printf("Product Price: ");
    scanf("%f", &newProd->price);
    printf("Product Quantity: ");
    scanf("%d", &newProd->quantity);

    *totalProducts = newCount;
    printf(" Product added successfully!\n");
    return 1;
}

void viewProducts(Product *inventory, int totalProducts) {
    if (totalProducts == 0) {
        printf("\nNo products in inventory.\n");
        return;
    }

    printf("\n========= PRODUCT LIST =========\n");
    for (int i = 0; i < totalProducts; i++) {
        printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
               inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
    }
}

void updateQuantity(Product *inventory, int totalProducts) {
    int id, qty, found = 0;
    printf("Enter Product ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < totalProducts; i++) {
        if (inventory[i].id == id) {
            printf("Enter new quantity: ");
            scanf("%d", &qty);
            inventory[i].quantity = qty;
            printf("Quantity updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Product not found!\n");
}

void searchByID(Product *inventory, int totalProducts) {
    int id, found = 0;
    printf("Enter Product ID: ");
    scanf("%d", &id);

    for (int i = 0; i < totalProducts; i++) {
        if (inventory[i].id == id) {
            printf("Found → ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
            found = 1;
            break;
        }
    }

    if (!found) printf("Product not found!\n");
}

void searchByName(Product *inventory, int totalProducts) {
    char query[NAME_SIZE];
    int found = 0;

    printf("Enter name to search (partial allowed): ");
    scanf("%49s", query);

    for (int i = 0; i < totalProducts; i++) {
        if (strstr(inventory[i].name, query)) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) printf("No products matched.\n");
}

void searchByPriceRange(Product *inventory, int totalProducts) {
    float minP, maxP;
    int found = 0;

    printf("Enter minimum price: ");
    scanf("%f", &minP);
    printf("Enter maximum price: ");
    scanf("%f", &maxP);

    for (int i = 0; i < totalProducts; i++) {
        if (inventory[i].price >= minP && inventory[i].price <= maxP) {
            printf("ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].id, inventory[i].name, inventory[i].price, inventory[i].quantity);
            found = 1;
        }
    }

    if (!found) printf("No products found in range.\n");
}

int deleteProduct(Product **inventory, int *totalProducts) {
    int id, idx = -1;
    printf("Enter Product ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < *totalProducts; i++) {
        if ((*inventory)[i].id == id) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Product not found!\n");
        return 0;
    }

    free((*inventory)[idx].name);

    for (int i = idx; i < *totalProducts - 1; i++) {
        (*inventory)[i] = (*inventory)[i + 1];
    }

    (*totalProducts)--;

    if (*totalProducts == 0) {
        free(*inventory);
        *inventory = NULL;
    } else {
        Product *temp = realloc(*inventory, (*totalProducts) * sizeof(Product));
        if (temp != NULL) {
            *inventory = temp;
        } else {
            printf("Warning: memory reallocation failed, but product deleted.\n");
        }
    }

    printf("Product deleted successfully!\n");
    return 1;
}

void freeInventory(Product *inventory, int totalProducts) {
    for (int i = 0; i < totalProducts; i++) {
        free(inventory[i].name);
    }
    free(inventory);
}



int main() {
    int totalProducts = 0, choice;
    Product *inventory = NULL;

    printf("Enter initial number of products: ");
    if (scanf("%d", &totalProducts) != 1 || totalProducts < 0) {
        printf("Invalid number of products.\n");
        return 1;
    }

    inventory = initializeInventory(totalProducts);
    for (int i = 0; i < totalProducts; i++) {
        inventory[i].name = malloc(NAME_SIZE);
        printf("\nEnter details for product %d:\n", i + 1);
        printf("Product ID: ");
        scanf("%d", &inventory[i].id);
        printf("Product Name: ");
        scanf("%49s", inventory[i].name);
        printf("Product Price: ");
        scanf("%f", &inventory[i].price);
        printf("Product Quantity: ");
        scanf("%d", &inventory[i].quantity);
    }

    while (1) {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search by ID\n");
        printf("5. Search by Name\n");
        printf("6. Search by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addProduct(&inventory, &totalProducts); break;
            case 2: viewProducts(inventory, totalProducts); break;
            case 3: updateQuantity(inventory, totalProducts); break;
            case 4: searchByID(inventory, totalProducts); break;
            case 5: searchByName(inventory, totalProducts); break;
            case 6: searchByPriceRange(inventory, totalProducts); break;
            case 7: deleteProduct(&inventory, &totalProducts); break;
            case 8:
                freeInventory(inventory, totalProducts);
                printf("Memory released. Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
}
