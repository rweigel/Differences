import numpy as np

r      = 3.0 # Radius in R_E
Ntheta = 90   # Number of polar angle points
Nphi   = 90   # Number of azimuthal (latitudinal angle) points

thetad = np.linspace(0,180,Ntheta)
phid = np.linspace(0,360,Nphi+1)
phid = phid[0:-1]

#print(thetad)
#print(phid)

theta = thetad*np.pi/180.0
phi = phid*np.pi/180.0

x = np.zeros((Ntheta*Nphi,))
y = np.zeros((Ntheta*Nphi,))
z = np.zeros((Ntheta*Nphi,))

i = 0

# Don't write redundant theta = 0 values if keepRedundant = False
# Less postprocessing manipulation needed if small number of redundant points kept.
keepRedundant = False
theta0 = False

for t in range(0,len(theta)):
    for p in range(0,len(phi)):
        
        if keepRedundant and theta0 and theta[t] == 0: continue # Already printed theta = 0 value

        x[i] = r*np.sin(theta[t])*np.cos(phi[p])
        y[i] = r*np.sin(theta[t])*np.sin(phi[p])
        z[i] = r*np.cos(theta[t])
        print('%f,%f,%f,%f,%f,%f' % (x[i],y[i],z[i],r,thetad[t],phid[p]))
        #print('%f,%f,%f' % (x[i],y[i],z[i]))
        i = i+1

        if keepRedundant and theta[t] == 0: theta0 = True # Printed a theta = 0 value
