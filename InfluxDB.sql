SELECT *
FROM "/CO2"
WHERE
time >= now() - interval '7 days'
