const companies= [
  {name: "Company One", category: "Finance", start: 1981, end: 2004, employees: 1000},
  {name: "Company Two", category: "Retail", start: 1992, end: 2008, employees: 2000},
  {name: "Company Three", category: "Auto", start: 1999, end: 2007, employees: 500},
  {name: "Company Four", category: "Retail", start: 1989, end: 2010, employees: 100},
  {name: "Company Five", category: "Technology", start: 2009, end: 2014, employees: 10000},
  {name: "Company Six", category: "Finance", start: 1987, end: 2010, employees: 200},
  {name: "Company Seven", category: "Auto", start: 1986, end: 1996, employees: 1000},
  {name: "Company Eight", category: "Technology", start: 2011, end: 2016, employees: 1000},
  {name: "Company Nine", category: "Retail", start: 1981, end: 1989, employees: 1000}
  ];

companies.forEach(company => console.log(`company name : ${company.name},category : ${company.category}`));

const retailCompanies = companies.filter(company=> company.category == "Retail");
console.log(retailCompanies);

const companies80s = companies.filter(company => company.start >= 1980 && company.start < 1990);
console.log(companies80s);

const companyNames = companies.map(company => company.name);
console.log(companyNames);

const companyDurations = companies.map(company => `${company.name} [${company.start} - ${company.end}]`);
console.log(companyDurations);

companies.sort((a, b) => a.start - b.start);
console.log(companies);

const totalEmployees = companies.reduce((total, company) => total + company.employees, 0);
console.log(totalEmployees);

const retailCompaniesBefore1985 = companies.filter(company => company.category === "Retail" && company.start < 1985)
  .sort((a, b) => b.start - a.start);
const totalRetailEmployeesBefore1985 = retailCompaniesBefore1985.reduce((total, company) => total + company.employees, 0);
console.log(retailCompaniesBefore1985);
console.log(totalRetailEmployeesBefore1985);