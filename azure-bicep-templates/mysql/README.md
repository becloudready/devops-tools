
## Flexible MySQL template

- Flexible Mysql Template
- Public Access ( open )

## Login

```
az login

```

## First do the what-if dry run ( similar to TF plan)

```
az deployment group what-if --name bicep-mysql --resource-group biceps-demo-rg --template-file flexible-mysql-vnet.bicep --parameters parameters.json
```

## Run the full deployment

```
az deployment group create --name bicep-mysql --resource-group biceps-demo-rg --template-file flexible-mysql-vnet.bicep --parameters parameters.json
```

## Check Information about the MySQL server deployed

```
az mysql flexible-server list
```

## Login to the MySQL server

```
 mysql -hbcrtest-mysql.mysql.database.azure.com -u bcruser -p
 
```

### Delete the deployment

```
az mysql flexible-server delete --name bcrtest-mysql --resource-group biceps-demo-rg --yes

```